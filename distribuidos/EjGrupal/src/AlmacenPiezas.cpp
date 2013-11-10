/* 
 * File:   AlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 1:46 PM
 */

#include <sstream>
#include <cstdlib>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "Common.h"
#include "API/AlmacenPiezas/ControladorAlmacenPiezas.h"
#include "API/AlmacenPiezas/IControladorAlmacenPiezas.h"
#include "API/AGV/ControladorAGV.h" 
#include "Logger/Logger.h"
#include "LockFile.h"

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

PedidoProduccion armarPedidoProduccion(pedido_fabricacion_t pedidoFabricacion) {
    PedidoProduccion pedido;
    pedido.cantidad = pedidoFabricacion.producidoParaStockear + pedidoFabricacion.producidoVendido;
    pedido.diferenciaMinima = pedidoFabricacion.diferenciaMinimaProducto;
    pedido.nroOrdenCompra = pedidoFabricacion.numOrdenCompra;
    pedido.tipo = pedidoFabricacion.tipoProducto;
    return pedido;
}

bool obtenerEspecificacionesDelProducto(TipoProducto tipoProducto, EspecifProd & piezas) {
    char buffer[TAM_BUFFER];
    ifstream stream;
    stream.open(NOMBRE_ARCHIVO_PRODUCTOS);
    Parser parser;
    sprintf(buffer, "Buscando especificaciones del producto %d", tipoProducto);
    Logger::logMessage(Logger::TRACE, buffer);
    
    piezas.idProducto = tipoProducto;
    if (!buscarUbiacionDeProductoEnArchivo (parser, stream, tipoProducto)) {
        Logger::logMessage(Logger::TRACE, "Error buscando especificaciones en el archivo");
        return false;
    }
    
    parser.obtenerProximoValor();
    parser.obtenerProximoValor();
    string cantidadPiezasString = parser.obtenerProximoValor();
    int cantPiezas = atoi(cantidadPiezasString.c_str());
    Logger::logMessage(Logger::ERROR, cantidadPiezasString);
    piezas.cantPiezas = 0;
    for (int i = 0; i < cantPiezas; i++) {
        int id = atoi(parser.obtenerProximoValor().c_str());
        int cantidad = atoi(parser.obtenerProximoValor().c_str());
        if (id < PANTALLA_1) {
            piezas.pieza[piezas.cantPiezas].tipoPieza = static_cast<TipoPieza> (id);
            piezas.pieza[piezas.cantPiezas].cantidad = cantidad;
            
            sprintf(buffer, "cantPiezas %d: Pieza a guardar %d en la posicion  %d Cantidad: %d", cantPiezas, piezas.pieza[piezas.cantPiezas].tipoPieza, piezas.cantPiezas, piezas.pieza[piezas.cantPiezas].cantidad);
            Logger::logMessage(Logger::ERROR, buffer);
            
            piezas.cantPiezas++;
            continue;
        }
        if (id >= PANTALLA_1) {
            sprintf(buffer, "cantPiezas %d: Gabinete a guardar %d, Cantidad: %d", cantPiezas, id, cantidad);
            Logger::logMessage(Logger::ERROR, buffer);
            piezas.tipoPantalla.tipoPieza = static_cast<TipoPieza> (id);
            piezas.tipoPantalla.cantidad = cantidad;
            
        }
    }
    
    for (int i = 0; i < piezas.cantPiezas; i++) {
        sprintf(buffer, "Estado piezas: Tipo: %d Cantidad: %d",piezas.pieza[i].tipoPieza,piezas.pieza[i].cantidad);
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
    }
    return true;
}

/**
 * @param tipoPieza
 * @param piezasReservadasTemporalmente
 * @param numAGV
 * @return position of the changed basket
 */
int avisarAAGVQueAgregueCanasto(std::auto_ptr<IControladorAlmacenPiezas> 
        controladorAlmacenPiezas, TipoPieza tipoPieza, EspecifProd 
        piezasReservadasTemporalmente[2], int numAGV, int cantPedidos, 
        int posicionesYaPedidas[MAX_PIEZAS_POR_PRODUCTO]) {
    char buffer[TAM_BUFFER];
    
    Logger::getInstance();
    sprintf(buffer, "Almacen piezas -:");
    Logger::setProcessInformation(buffer);
    BufferCanastos canastos;
    bool canastoPresente = false;
    int posTemp;

    canastos = controladorAlmacenPiezas->obtenerBufferCanastos(numAGV);
    

    // j recorre el buffer de los canastos
    int j = 0;
    while (!canastoPresente && j < MAX_QUANTITY_CANASTOS) {
        canastoPresente = canastos.canastos[j].tipoPieza == tipoPieza;
        j++;
    }
    if (canastoPresente) {
        sprintf(buffer, "el canasto del tipo de pieza %d esta presente en la posicion %d", tipoPieza, j);
        Logger::getInstance().logMessage(Logger::DEBUG, buffer);
        return -1;
    }
    // esta mal este buscar, no contempla los envios anteriores a los AGVs!!!!!!!!!!!!!!!

    for (int posCanasto = 0; posCanasto < MAX_QUANTITY_CANASTOS; posCanasto++) {
        // k es para comparar con ambos conjuntos de piezas utilizados anteriormente
        for (int k = 0; k < 2; k++) {
            if (piezasReservadasTemporalmente[k].idProducto == NULL_PRODUCT)
                continue;
            // estoy chequeando contra los robots 12, usan todas las piezas, los robots 11 solo usan la pantalla
            if (numAGV == 1) {
                int posPiezaReservadaTemporalmente = 0;
                while (!canastoPresente && posPiezaReservadaTemporalmente < piezasReservadasTemporalmente[k].cantPiezas) {
                    canastoPresente = canastos.canastos[posCanasto].tipoPieza == piezasReservadasTemporalmente[k].pieza[posPiezaReservadaTemporalmente].tipoPieza;
                    posPiezaReservadaTemporalmente++;
                }
            } else
                canastoPresente = canastos.canastos[posCanasto].tipoPieza == piezasReservadasTemporalmente[k].tipoPantalla.tipoPieza;
        }
        
        //chequea que la posicion del canasto que voy a pedir no la haya pedido previamente
        if (numAGV == 1) {
            posTemp = 0;
            sprintf(buffer, "antes del while!!!!!  pos a pedir %d, cant pedidos %d", posCanasto, cantPedidos);
            Logger::getInstance().logMessage(Logger::IMPORTANT, buffer);
            while(!canastoPresente && posTemp < cantPedidos) {
                sprintf(buffer, "pos a pedir %d, pos pedida = %d", posCanasto, posicionesYaPedidas[posTemp]);
                Logger::getInstance().logMessage(Logger::DEBUG, buffer);

                canastoPresente = posCanasto == posicionesYaPedidas[posTemp];
                posTemp++;
            }
        }
        if (!canastoPresente) {
            sprintf(buffer, "cambio el canasto del buffer %d que tiene la pieza"
                    " %d por Pieza: %d en el lugar %d", numAGV, canastos
                    .canastos[posCanasto].tipoPieza, tipoPieza, posCanasto);
            Logger::getInstance().logMessage(Logger::DEBUG, buffer);
            controladorAlmacenPiezas->avisarAAGVQueAgregueCanasto(numAGV, posCanasto, tipoPieza);
            if (numAGV == 1)
                posicionesYaPedidas[cantPedidos] = posCanasto;
            
            return posCanasto;
        }
    }
    sprintf(buffer, "si llegue aca esta todo mallll");
    Logger::getInstance().logMessage(Logger::DEBUG, buffer);
    return -1;
}


int avisarAAGVQueAgregueCanasto(std::auto_ptr<IControladorAlmacenPiezas> 
        controladorAlmacenPiezas, TipoPieza tipoPieza, EspecifProd 
        piezasReservadasTemporalmente[2], int numAGV) {
    
    int pos[MAX_PIEZAS_POR_PRODUCTO];
    return avisarAAGVQueAgregueCanasto(controladorAlmacenPiezas, tipoPieza, piezasReservadasTemporalmente, numAGV, 0, pos);
}

int main(int argc, char** argv)
{
    std::auto_ptr<IControladorAlmacenPiezas> controladorAlmacenPiezas = std::auto_ptr<IControladorAlmacenPiezas>(new ControladorAlmacenPiezas());

    pedido_fabricacion_t pedidoFabricacion;
    PedidoProduccion pedidoProduccion;
    EspecifProd piezasProductoActual;
    EspecifProd piezasReservadasTemporalmente[2];
    piezasReservadasTemporalmente[0].idProducto = NULL_PRODUCT;
    piezasReservadasTemporalmente[1].idProducto = NULL_PRODUCT;
    int posicionesYaPedidas[MAX_PIEZAS_POR_PRODUCTO];
    int posCanasto;
    int cantCanastosPedidos;
    char buffer[TAM_BUFFER];
    
    while (true) {
        pedidoFabricacion = controladorAlmacenPiezas->recibirPedidoDeFabricacion();
        pedidoProduccion = armarPedidoProduccion (pedidoFabricacion);
        controladorAlmacenPiezas->enviarPedidoProduccionARobot5(pedidoProduccion);
        
        if (!obtenerEspecificacionesDelProducto(pedidoFabricacion.tipoProducto, piezasProductoActual))
            abort();
        
        for (int i = 0; i < piezasProductoActual.cantPiezas; i++) {
            sprintf(buffer, "pieza destinada a robot 12: %d, con cantidad %d", piezasProductoActual.pieza[i].tipoPieza, piezasProductoActual.pieza[i].cantidad);
            Logger::getInstance().logMessage(Logger::DEBUG, buffer);
        }
        sprintf(buffer, "pieza destinada a robot 11: %d, con cantidad %d", piezasProductoActual.tipoPantalla.tipoPieza, piezasProductoActual.tipoPantalla.cantidad);
        Logger::getInstance().logMessage(Logger::DEBUG, buffer);
        
        cantCanastosPedidos = 0;
        for (int i = 0; i < piezasProductoActual.cantPiezas; i++) {
            int numAGV = 1;
            posCanasto = avisarAAGVQueAgregueCanasto (controladorAlmacenPiezas, 
                    piezasProductoActual.pieza[i].tipoPieza, 
                    piezasReservadasTemporalmente, numAGV, i, posicionesYaPedidas);
            if (posCanasto != -1) {
                posicionesYaPedidas[cantCanastosPedidos] = posCanasto;
                cantCanastosPedidos++;
            }
        }
        for (int numAGV = 0; numAGV < CANTIDAD_AGVS; numAGV +=2) {
            avisarAAGVQueAgregueCanasto (controladorAlmacenPiezas, 
                    piezasProductoActual.tipoPantalla.tipoPieza, 
                    piezasReservadasTemporalmente, numAGV);
        }
        
        memcpy(piezasReservadasTemporalmente, piezasReservadasTemporalmente + 1, sizeof(EspecifProd));
        memcpy(piezasReservadasTemporalmente + 1, &piezasProductoActual, sizeof(EspecifProd));
        controladorAlmacenPiezas->recibirConfirmacionProduccion();
    } 
}

