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
    piezas.cantPiezas = 0;
    for (int i = 0; i < cantPiezas; i++) {
        int id = atoi(parser.obtenerProximoValor().c_str());
        int cantidad = atoi(parser.obtenerProximoValor().c_str());
        if (id < PANTALLA_1) {
            piezas.pieza[piezas.cantPiezas].tipoPieza = static_cast<TipoPieza> (id);
            piezas.pieza[piezas.cantPiezas].cantidad = cantidad;
            
            sprintf(buffer, "cantPiezas %d: Pieza a guardar %d en la posicion  %d Cantidad: %d", cantPiezas, piezas.pieza[piezas.cantPiezas].tipoPieza, piezas.cantPiezas, piezas.pieza[piezas.cantPiezas].cantidad);
            Logger::logMessage(Logger::DEBUG, buffer);
            
            piezas.cantPiezas++;
            continue;
        }
        if (id >= PANTALLA_1) {
            sprintf(buffer, "cantPiezas %d: Gabinete a guardar %d, Cantidad: %d", cantPiezas, id, cantidad);
            Logger::logMessage(Logger::DEBUG, buffer);
            piezas.tipoPantalla.tipoPieza = static_cast<TipoPieza> (id);
            piezas.tipoPantalla.cantidad = cantidad;
            
        }
    }
    
    for (int i = 0; i < piezas.cantPiezas; i++) {
        sprintf(buffer, "Estado piezas: Tipo: %d Cantidad: %d",piezas.pieza[i].tipoPieza,piezas.pieza[i].cantidad);
        Logger::getInstance().logMessage(Logger::DEBUG, buffer);
    }
    return true;
}

/**
 * @param tipoPieza
 * @param piezasReservadasTemporalmente
 * @param numAGV
 * @return position of the changed basket
 */
int avisarAAGVQueAgregueCanasto(std::auto_ptr<IControladorAlmacenPiezas>& 
        controladorAlmacenPiezas, TipoPieza tipoPieza, EspecifProd 
        piezasReservadasTemporalmente[2], int numAGV, int cantPedidos, 
        int posicionesYaPedidas[MAX_PIEZAS_POR_PRODUCTO], EspecifProd 
        piezasProductoActual) {
    char buffer[TAM_BUFFER];
    
    sprintf(buffer, "chequeando si el canasto con la pieza %d esta a los pies del AGV %d", tipoPieza, numAGV);
    Logger::logMessage(Logger::DEBUG, buffer);
    
    Logger::getInstance();
    sprintf(buffer, "Almacen piezas -:");
    Logger::setProcessInformation(buffer);
    BufferCanastos canastos;
    bool canastoPresente = false;
    bool posPedida = false;
    int posTemp;

    canastos = controladorAlmacenPiezas->obtenerBufferCanastos(numAGV);

    // j recorre el buffer de los canastos
    int j = 0;
    while (!canastoPresente && j < MAX_QUANTITY_CANASTOS) {
        canastoPresente = canastos.canastos[j].tipoPieza == tipoPieza;
        j++;
    }
    if (canastoPresente) {
        sprintf(buffer, "el canasto del tipo de pieza %d esta presente en la posicion %d", tipoPieza, j - 1);
        Logger::logMessage(Logger::DEBUG, buffer);
        return -1;
    }
    // esta mal este buscar, no contempla los envios anteriores a los AGVs!!!!!!!!!!!!!!!
    int posCanasto = 0;
    while (!canastoPresente && posCanasto < MAX_QUANTITY_CANASTOS) {
        posPedida = false;
        
        // k es para comparar con ambos conjuntos de piezas utilizados anteriormente
        for (int k = 0; k < 2; k++) {
            if (piezasReservadasTemporalmente[k].idProducto == NULL_PRODUCT) {
                sprintf(buffer, "pedido anterior Nº %d, no fue realizado", k);
                Logger::logMessage(Logger::DEBUG, buffer);
                continue;
            }
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
            while(!posPedida && posTemp < cantPedidos) {
                sprintf(buffer, "posicion ya pedida %d en posicion %d, posicion a pedir %d, Cantidad Pedidos %d", posicionesYaPedidas[posTemp], posTemp, posCanasto, cantPedidos);
                Logger::logMessage(Logger::DEBUG, buffer);
                posPedida = posCanasto == posicionesYaPedidas[posTemp];
                posTemp++;
            }
            // falta chequear que la posicion pedida no posea piezas de las que voy a necesitar en la misma orden de compra
            posTemp = 0;
            while(!posPedida && posTemp < piezasProductoActual.cantPiezas) {
                sprintf(buffer, "tipo de pieza a reemplazar %d en la posicion %d de los canastos %d, pieza a agregar %d", canastos.canastos[posCanasto].tipoPieza, posCanasto, numAGV, piezasProductoActual.pieza[posTemp].tipoPieza);
                Logger::logMessage(Logger::DEBUG, buffer);
                posPedida = (canastos.canastos[posCanasto].tipoPieza == piezasProductoActual.pieza[posTemp].tipoPieza);
                posTemp++;
            }
        }
        
        if (!canastoPresente && !posPedida) {
            sprintf(buffer, "cambio el canasto del buffer %d que tiene la pieza"
                    " %d por Pieza: %d en el lugar %d", numAGV, canastos
                    .canastos[posCanasto].tipoPieza, tipoPieza, posCanasto);
            Logger::getInstance().logMessage(Logger::DEBUG, buffer);
            PedidoCanastoRobotCinta6 pedidoCanasto;
            pedidoCanasto.lugar = posCanasto;
            pedidoCanasto.tipoPieza = tipoPieza;
            controladorAlmacenPiezas->avisarAAGVQueAgregueCanasto(numAGV, pedidoCanasto);
            if (numAGV == 1)
                posicionesYaPedidas[cantPedidos] = posCanasto;
            
            return posCanasto;
        }
        posCanasto++;
    }
    return -1;
}


int avisarAAGVQueAgregueCanasto(std::auto_ptr<IControladorAlmacenPiezas>& 
        controladorAlmacenPiezas, TipoPieza tipoPieza, EspecifProd 
        piezasReservadasTemporalmente[2], int numAGV) {
    
    int pos[MAX_PIEZAS_POR_PRODUCTO];
    EspecifProd piezasProd;
    return avisarAAGVQueAgregueCanasto(controladorAlmacenPiezas, tipoPieza, piezasReservadasTemporalmente, numAGV, 0, pos, piezasProd);
}

int main(int argc, char** argv)
{
    std::auto_ptr<IControladorAlmacenPiezas> controladorAlmacenPiezas (new ControladorAlmacenPiezas());

    pedido_fabricacion_t pedidoFabricacion;
    PedidoProduccion pedidoProduccion;
    EspecifProd piezasProductoActual;
    EspecifProd piezasReservadasTemporalmente[2];
    piezasReservadasTemporalmente[0].idProducto = NULL_PRODUCT;
    piezasReservadasTemporalmente[1].idProducto = NULL_PRODUCT;
    int posicionesYaPedidas[MAX_PIEZAS_POR_PRODUCTO];
    int posCanasto;
    int cantCanastosPedidos;
    
    while (true) {
        pedidoFabricacion = controladorAlmacenPiezas->recibirPedidoDeFabricacion();
        pedidoProduccion = armarPedidoProduccion (pedidoFabricacion);
        controladorAlmacenPiezas->enviarPedidoProduccionARobot5(pedidoProduccion);
        
        if (!obtenerEspecificacionesDelProducto(pedidoFabricacion.tipoProducto, piezasProductoActual)) {
            Logger::getInstance().logMessage(Logger::ERROR, "No se pudo obtener la especificacion... ABORTANDO!!!!");
            abort();
        }

        cantCanastosPedidos = 0;
        for (int i = 0; i < piezasProductoActual.cantPiezas; i++) {
            int numAGV = 1;
            posCanasto = avisarAAGVQueAgregueCanasto (controladorAlmacenPiezas, 
                    piezasProductoActual.pieza[i].tipoPieza, 
                    piezasReservadasTemporalmente, numAGV, i, posicionesYaPedidas, piezasProductoActual);
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

