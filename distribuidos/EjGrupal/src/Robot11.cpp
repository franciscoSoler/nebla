/* 
 * File:   Robot11.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:30
 */
#include <sstream>
#include <cstdlib>
#include <memory>
#include "Common.h"
#include "API/Robot11/IControladorRobot11.h"
#include "API/Robot11/ControladorRobot11.h"
#include "Parser/Parser.h"

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

int buscarPosicionPieza(int id_Robot, BufferCanastos canastos, int id_pieza) {
    char buffer[TAM_BUFFER];
    
    sprintf(buffer, "Robot 11-%u - buscarPosicionPieza:", id_Robot);
    Logger::setProcessInformation(buffer);
    sprintf(buffer, "busco en que posicion esta la pieza %d", id_pieza);
    Logger::logMessage(Logger::TRACE, buffer);
    for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++)
        if (canastos.canastos[i].tipoPieza == id_pieza) {
            return i;
        }
    return -1;
}

bool poseePieza(std::auto_ptr<IControladorRobot11>& controladorRobot11, int id_Robot, int id_pieza, BufferCanastos& canastos, int& posicionPieza) {
    char buffer[TAM_BUFFER];
    
    sprintf(buffer, "Robot 11-%u - poseePieza:", id_Robot);
    Logger::setProcessInformation(buffer);

    while (true) {
        canastos = controladorRobot11->obtenerBufferCanastos();
        posicionPieza = buscarPosicionPieza(id_Robot, canastos, id_pieza);

        if (posicionPieza != -1) {
            sprintf(buffer, "El canasto está presente en la posicion %d", posicionPieza);
            Logger::logMessage(Logger::TRACE, buffer);
            if (canastos.canastos[posicionPieza].cantidadPiezas != 0)
                return true;

            Logger::logMessage(Logger::TRACE, "no hay piezas en el canasto, aviso que posicion estare esperando y devuelvo mem Canastos");
            controladorRobot11->pedirPiezaFaltante(id_Robot, canastos, posicionPieza);                                                                        
            return false;
        } else {
            Logger::logMessage(Logger::TRACE, "canasto no presente, aviso que posicion espero, devuelvo mem canastos y me duermo");
            controladorRobot11->pedirCanastoFaltante(id_Robot, canastos, -2);
        }
    }
}

bool agregarPantalla(std::auto_ptr<IControladorRobot11>& controladorRobot11, int id_Robot, EspecifPiezaDeProd piezaDeProd, int& posicionPieza) {
    char buffer[TAM_BUFFER];
    BufferCanastos canastos;
    
    sprintf(buffer, "Robot 11-%u - agregarPantalla:", id_Robot);
    Logger::setProcessInformation(buffer);

    if(!poseePieza(controladorRobot11, id_Robot, piezaDeProd.tipoPieza, canastos, posicionPieza))
        return false;

    sprintf(buffer, "la cantidad de piezas que hay en el canasto son %d", canastos.canastos[posicionPieza].cantidadPiezas);
    Logger::logMessage(Logger::TRACE, buffer);
    int piezasRetiradas = std::min(rand() % 3 + 1, canastos.canastos[posicionPieza].cantidadPiezas);
    canastos.canastos[posicionPieza].cantidadPiezas -= piezasRetiradas;
    
    controladorRobot11->posicionarCanasto(canastos);
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
    
    std::auto_ptr<IControladorRobot11> controladorRobot11 (new ControladorRobot11());
    controladorRobot11->iniciarControlador(nroRobot);
    Caja unaCaja;
    EspecifProd piezas;
    int posicionPieza;
    
    while (true) {
        while (controladorRobot11->buscarProximoGabinete(&piezas) == false)
            controladorRobot11->avanzarCinta(); 
            
        //for (int i = 0; i < piezas.cantPiezas; i++) {
            while (true) { // anteriormente tenia tipo de pieza
                //if (!controladorRobot11->agregarPantalla(piezas.tipoPantalla))
                if (!agregarPantalla(controladorRobot11, nroRobot, piezas.tipoPantalla, posicionPieza))
                    controladorRobot11->pedirPiezaAlAGV(piezas.tipoPantalla.tipoPieza, posicionPieza);
                    //controladorRobot11->pedirPiezaAlAGV(piezas.tipoPantalla.tipoPieza);
                if (piezas.tipoPantalla.cantidad-- == 0)
                    break;
            }            
        //}
        unaCaja = controladorRobot11->cerrarYTomarCaja();
        controladorRobot11->depositarCaja(unaCaja);
    }
    return 0;
}

