/* 
 * File:   Robot12.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:29
 */
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "Common.h"
#include "API/Robot12/IControladorRobot12.h"
#include "API/Robot12/ControladorRobot12.h"
#include "Parser/Parser.h"
#include "Logger/Logger.h"

bool buscarUbiacionDeProductoEnArchivo(Parser &parser, ifstream& stream, int numeroProducto)
{
    int ultimoNumeroProductoLeido = 0;
    do
    {
	if(!parser.obtenerLineaSiguiente(stream))
	    return false;
	string ultimoNumeroProductoLeidoString = parser.obtenerProximoValor();
	ultimoNumeroProductoLeido = atoi(ultimoNumeroProductoLeidoString.c_str());
    } while(ultimoNumeroProductoLeido != numeroProducto);
    return true;
}

bool obtenerPiezasDelProducto(TipoProducto tipoProducto, EspecifProd *piezas) {
    ifstream stream;
    stream.open(NOMBRE_ARCHIVO_PRODUCTOS);
    Parser parser;
    
    if (!buscarUbiacionDeProductoEnArchivo (parser, stream, tipoProducto))
        return false;
    
    parser.obtenerProximoValor();
    parser.obtenerProximoValor();
    string cantidadPiezasString = parser.obtenerProximoValor();
    int cantPiezas = atoi(cantidadPiezasString.c_str());
    piezas->cantPiezas = 0;
    for (int i = 0; i < cantPiezas*2; i+=2) {
        int id = atoi(parser.obtenerProximoValor().c_str());
        int cantidad = atoi(parser.obtenerProximoValor().c_str());
        if (id < PANTALLA_1) {
            piezas->pieza[i].tipoPieza = static_cast<TipoPieza> (id);
            piezas->pieza[i].cantidad = cantidad;
            piezas->cantPiezas++;
        }
    }
    return true;
}

int buscarPosicionPieza(int id_Robot, BufferCanastos canastos, int id_pieza) {
    char buffer[TAM_BUFFER];
    
    sprintf(buffer, "Robot 12-%u - buscarPosicionPieza:", id_Robot);
    Logger::setProcessInformation(buffer);
    sprintf(buffer, "busco en que posicion esta la pieza %d", id_pieza);
    Logger::logMessage(Logger::TRACE, buffer);
    for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++)
        if (canastos.canastos[i].tipoPieza == id_pieza) {
            return i;
        }
    return -1;
}

bool poseePieza(std::auto_ptr<IControladorRobot12>& controladorRobot12, int id_Robot, int id_pieza, BufferCanastos& canastos, int& posicionPieza) {
    char buffer[TAM_BUFFER];
    
    sprintf(buffer, "Robot 12-%u - poseePieza:", id_Robot);
    Logger::setProcessInformation(buffer);

    while (true) {
        canastos = controladorRobot12->obtenerBufferCanastos();
        posicionPieza = buscarPosicionPieza(id_Robot, canastos, id_pieza);

        sprintf(buffer, "Robot 12-%u - poseePieza:", id_Robot);
        Logger::setProcessInformation(buffer);
        if (posicionPieza != -1) {
            sprintf(buffer, "esta el canasto presente en la posicion %d", posicionPieza);
            Logger::logMessage(Logger::TRACE, buffer);
            if (canastos.canastos[posicionPieza].cantidadPiezas != 0)
                return true;

            Logger::logMessage(Logger::TRACE, "no hay piezas en el canasto, aviso que posicion estare esperando y devuelvo mem Canastos");
            controladorRobot12->pedirPiezaFaltante(id_Robot, canastos, posicionPieza);                                                                        
            return false;
        } else {
            Logger::logMessage(Logger::TRACE, "canasto no presente, aviso que posicion espero, devuelvo mem canastos y me duermo");
            controladorRobot12->pedirCanastoFaltante(id_Robot, canastos, -2);
        }
    }
}

bool agregarConector(std::auto_ptr<IControladorRobot12>& controladorRobot12, int id_Robot, EspecifPiezaDeProd piezaDeProd, int& posicionPieza) {
    char buffer[TAM_BUFFER];
    BufferCanastos canastos;
    
    sprintf(buffer, "Robot 12-%u - agregarConector:", id_Robot);
    Logger::setProcessInformation(buffer);
    if(!poseePieza(controladorRobot12, id_Robot, piezaDeProd.tipoPieza, canastos, posicionPieza))
        return false;

    sprintf(buffer, "la cantidad de piezas que hay en el canasto son %d", canastos.canastos[posicionPieza].cantidadPiezas);
    Logger::logMessage(Logger::TRACE, buffer);
    int piezasRetiradas = std::min(rand() % 3 + 1, canastos.canastos[posicionPieza].cantidadPiezas);
    canastos.canastos[posicionPieza].cantidadPiezas -= piezasRetiradas;
    
    controladorRobot12->posicionarCanasto(canastos);
    sleep(rand() %5 + 1);
    return true;
}


/*
 * 
 */
int main(int argc, char** argv) {
    std::stringstream ss;
    int nroRobot;
    ss << argv[1];
    ss >> nroRobot;
    
    std::auto_ptr<IControladorRobot12> controladorRobot12 (new ControladorRobot12());
    controladorRobot12->iniciarControlador(nroRobot);
    CintaTransportadora_6 ctrlCinta;
    EspecifProd piezas;
    int posicionPieza;
    while (true) {
        ctrlCinta = controladorRobot12->esperarProximoGabinete(); 
        
        obtenerPiezasDelProducto(ctrlCinta.productoProduccion[ctrlCinta.puntoLectura].tipoProducto, &piezas);
        
        for (int i = 0; i < piezas.cantPiezas; i++) {
            while (true) {
                //if (!controladorRobot12->agregarConector(piezas.pieza[i]))
                if (!agregarConector(controladorRobot12, nroRobot, piezas.pieza[i], posicionPieza))
                    controladorRobot12->pedirPiezaAlAGV(piezas.pieza[i].tipoPieza, posicionPieza);
                if (piezas.pieza[i].cantidad-- == 0)
                    break;
            }
        }
        controladorRobot12->finalizarEnsamble();
    }
    return 0;
}

