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

#define CANTIDAD_TIPO_PIEZAS_RESERVADAS         3

typedef struct {
    EspecifProd piezasUtilizadas[CANTIDAD_TIPO_PIEZAS_RESERVADAS];
    int cantPiezasPedidas[CANTIDAD_TIPO_PIEZAS_RESERVADAS];
    int posicionesPiezasPedidas[CANTIDAD_TIPO_PIEZAS_RESERVADAS][MAX_PIEZAS_POR_PRODUCTO];
    int posicionesGabinetesPedidas[CANTIDAD_TIPO_PIEZAS_RESERVADAS];
} PedidosRealizados;


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
        controladorAlmacenPiezas, TipoPieza piezaAGuardar, 
        PedidosRealizados pedidosRealizados, int posProductoParaArmar, int numAGV) {
    char buffer[TAM_BUFFER];
    
    sprintf(buffer, "chequeando si el canasto con la pieza %d esta a los pies del AGV %d", piezaAGuardar, numAGV);
    Logger::logMessage(Logger::DEBUG, buffer);
    
    Logger::getInstance();
    sprintf(buffer, "Almacen piezas -:");
    Logger::setProcessInformation(buffer);
    BufferCanastos canastos;
    bool canastoPresente = false;
    bool posPedida = false;

    canastos = controladorAlmacenPiezas->obtenerBufferCanastos(numAGV);

    // j recorre el buffer de los canastos
    int j = 0;
    while (!canastoPresente && j < MAX_QUANTITY_CANASTOS) {
        canastoPresente = canastos.canastos[j].tipoPieza == piezaAGuardar;
        j++;
    }
    if (canastoPresente) {
        sprintf(buffer, "el canasto del tipo de pieza %d esta presente en la posicion %d", piezaAGuardar, j - 1);
        Logger::logMessage(Logger::DEBUG, buffer);
        return -1;
    }
    
    sprintf(buffer, "el canasto del tipo de pieza %d no esta en los canastos, chequeo que no haya sido pedido con anterioridad", piezaAGuardar);
    Logger::logMessage(Logger::DEBUG, buffer);
    
    //chequear que no fue pedido anteriormente el canasto en los pedidos anteriores
    int k = 0;
    while (!canastoPresente && k < CANTIDAD_TIPO_PIEZAS_RESERVADAS) {
        if (pedidosRealizados.piezasUtilizadas[k].idProducto == NULL_PRODUCT) {
            sprintf(buffer, "pedido Nº %d, no fue realizado", k);
            Logger::logMessage(Logger::DEBUG, buffer);
            k++;
            continue;
        }
        if (k == posProductoParaArmar) {
            k++;
            continue;
        }

        if (numAGV == 1) {
            j = 0;
            while (!canastoPresente && j < pedidosRealizados.piezasUtilizadas[k].cantPiezas) {
                canastoPresente = piezaAGuardar == pedidosRealizados.piezasUtilizadas[k].pieza[j].tipoPieza;
                j++;
            }
        } else
            canastoPresente = piezaAGuardar == pedidosRealizados.piezasUtilizadas[k].tipoPantalla.tipoPieza;
        k++;
    }
    if (!canastoPresente) {
        sprintf(buffer, "el canasto del tipo de pieza %d no fue pedido con anterioridad, busco posicion a reemplazar", piezaAGuardar);
        Logger::logMessage(Logger::DEBUG, buffer);
    }
    
    /* ahora hay que chequear las posiciones elegidas, tienen que cumplir las siguientes restricciones:
     *  posicionElegida != posicionYaElegida
     *  canasto de posicionElegida != piezas en productoParaArmar
     * */
    int posCanasto = 0;
    while (!canastoPresente && posCanasto < MAX_QUANTITY_CANASTOS) {
        posPedida = false;
        
        // primero busco un canasto que no pertenezca al productoActual
        if (numAGV == 1) {
            j = 0;
            while (!posPedida && j < pedidosRealizados.piezasUtilizadas[posProductoParaArmar].cantPiezas) {
                posPedida = canastos.canastos[posCanasto].tipoPieza == pedidosRealizados.piezasUtilizadas[posProductoParaArmar].pieza[j].tipoPieza;
                j++;
            }
        } else
            posPedida = canastos.canastos[posCanasto].tipoPieza == pedidosRealizados.piezasUtilizadas[posProductoParaArmar].tipoPantalla.tipoPieza;
        
        // chequeo que la posicion no haya sido pedida con anterioridad
        k = 0;
        while (!posPedida && k < CANTIDAD_TIPO_PIEZAS_RESERVADAS) {
            if (pedidosRealizados.piezasUtilizadas[k].idProducto == NULL_PRODUCT) {
                sprintf(buffer, "en chequear posiciones, pedido Nº %d, no fue realizado", k);
                Logger::logMessage(Logger::DEBUG, buffer);
                k++;
                continue;
            }
            
            
            if (numAGV == 1) {
                int posPiezaReservadaTemporalmente = 0;
                while (!posPedida && posPiezaReservadaTemporalmente < pedidosRealizados.cantPiezasPedidas[k]) {
                    sprintf(buffer, "pedido Nº %d: tipo de pieza a reemplazar %d"
                            " en la posicion %d de los canastos %d, pieza a "
                            "agregar %d", k, canastos.canastos[posCanasto].
                            tipoPieza, posCanasto, numAGV, piezaAGuardar);
                    Logger::logMessage(Logger::DEBUG, buffer);
                    
                    posPedida = posCanasto == pedidosRealizados.posicionesPiezasPedidas[k][posPiezaReservadaTemporalmente];
                    posPiezaReservadaTemporalmente++;
                }
            } else {
                sprintf(buffer, "pedido Nº %d: tipo de pieza a reemplazar %d"
                            " en la posicion %d de los canastos %d, pieza a "
                            "agregar %d", k, canastos.canastos[posCanasto].
                            tipoPieza, posCanasto, numAGV, piezaAGuardar);
                    Logger::logMessage(Logger::DEBUG, buffer);
                posPedida = posCanasto == pedidosRealizados.posicionesGabinetesPedidas[k];
            }
            
            k++;
        }
        
        if (!posPedida) {
            sprintf(buffer, "cambio el canasto del buffer %d que tiene la pieza"
                    " %d por Pieza: %d en el lugar %d", numAGV, canastos
                    .canastos[posCanasto].tipoPieza, piezaAGuardar, posCanasto);
            Logger::getInstance().logMessage(Logger::DEBUG, buffer);
            PedidoCanastoRobotCinta6 pedidoCanasto;
            pedidoCanasto.lugar = posCanasto;
            pedidoCanasto.tipoPieza = piezaAGuardar;
            controladorAlmacenPiezas->avisarAAGVQueAgregueCanasto(numAGV, pedidoCanasto);
            return posCanasto;
        }
        posCanasto++;
    }
    sprintf(buffer, "el canasto del tipo de pieza %d ya fue pedido con anterioridad", piezaAGuardar);
    Logger::logMessage(Logger::DEBUG, buffer);
    return -1;
}

int main(int argc, char** argv)
{
    std::auto_ptr<IControladorAlmacenPiezas> controladorAlmacenPiezas (new ControladorAlmacenPiezas());

    pedido_fabricacion_t pedidoFabricacion;
    PedidoProduccion pedidoProduccion;
    
    PedidosRealizados pedidosRealizados;
    pedidosRealizados.piezasUtilizadas[0].idProducto = NULL_PRODUCT;
    pedidosRealizados.piezasUtilizadas[1].idProducto = NULL_PRODUCT;
    pedidosRealizados.piezasUtilizadas[2].idProducto = NULL_PRODUCT;
    
    int posCanasto;
    int numPedido = 0;
    while (true) {
        pedidoFabricacion = controladorAlmacenPiezas->recibirPedidoDeFabricacion();
        pedidoProduccion = armarPedidoProduccion (pedidoFabricacion);
        controladorAlmacenPiezas->enviarPedidoProduccionARobot5(pedidoProduccion);
        
        pedidosRealizados.piezasUtilizadas[numPedido].idProducto = pedidoFabricacion.tipoProducto;
        pedidosRealizados.cantPiezasPedidas[numPedido] = 0;
        if (!obtenerEspecificacionesDelProducto(pedidoFabricacion.tipoProducto, pedidosRealizados.piezasUtilizadas[numPedido])) {
            Logger::getInstance().logMessage(Logger::ERROR, "No se pudo obtener la especificacion... ABORTANDO!!!!");
            abort();
        }
        
        for (int i = 0; i < pedidosRealizados.piezasUtilizadas[numPedido].cantPiezas; i++) {
            int numAGV = 1;
            posCanasto = avisarAAGVQueAgregueCanasto (controladorAlmacenPiezas, 
                    pedidosRealizados.piezasUtilizadas[numPedido].pieza[i].tipoPieza, 
                    pedidosRealizados, numPedido, numAGV); 
            if (posCanasto != -1) {
                pedidosRealizados.posicionesPiezasPedidas[numPedido][pedidosRealizados.cantPiezasPedidas[numPedido]] = posCanasto;
                pedidosRealizados.cantPiezasPedidas[numPedido]++;
            }
        }
        
        for (int numAGV = 0; numAGV < CANTIDAD_AGVS; numAGV +=2) {
            posCanasto = avisarAAGVQueAgregueCanasto (controladorAlmacenPiezas, 
                    pedidosRealizados.piezasUtilizadas[numPedido].tipoPantalla.tipoPieza, 
                    pedidosRealizados, numPedido, numAGV);
            if (posCanasto != -1) {
                pedidosRealizados.posicionesGabinetesPedidas[numPedido] = posCanasto;
            }
        }
        controladorAlmacenPiezas->recibirConfirmacionProduccion();
        
        numPedido = (numPedido + 1) % CANTIDAD_TIPO_PIEZAS_RESERVADAS;
    } 
}

