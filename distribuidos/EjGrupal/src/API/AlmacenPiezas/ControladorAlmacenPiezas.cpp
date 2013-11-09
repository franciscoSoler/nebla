/* 
 * File:   ControladorAlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 1:47 PM
 */

#include "ControladorAlmacenPiezas.h"
#include "../../Logger/Logger.h"
#include "LockFile.h"

ControladorAlmacenPiezas::ControladorAlmacenPiezas() :
        colaEnvioMensajePedidoProduccion ("Mensage Robot 5 Msg Queue")
{ 
    Logger::setProcessInformation("AlmacenDePiezas:");
    this->colaReciboOrdenProduccion = Cola<mensaje_pedido_fabricacion_t>(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);
    colaReciboOrdenProduccion.obtener();
    
    colaEnvioMensajePedidoProduccion.getMessageQueue(DIRECTORY_ROBOT_5, ID_COLA_PEDIDOS_PRODUCCION);
    
    this->colaPedidosCanastos = IPC::PedidosCanastosMessageQueue("colaPedidosCanastos");
    this->colaPedidosCanastos.getMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);
        
    this->shMemBufferCanastos[CANTIDAD_AGVS] = IPC::BufferCanastosSharedMemory("shMemBufferCanastos");
    this->shMemBufferCanastos[0].getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_0);
    this->shMemBufferCanastos[1].getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_1);
    this->shMemBufferCanastos[2].getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_2);
    
    this->semMemCanastos = IPC::Semaphore("semMemCanastos");
    this->semMemCanastos.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, 3);
}

ControladorAlmacenPiezas::~ControladorAlmacenPiezas() { }

pedido_fabricacion_t ControladorAlmacenPiezas::recibirPedidoDeFabricacion()
{
    mensaje_pedido_fabricacion_t mensajePedido;
    this->colaReciboOrdenProduccion.recibir(1, &mensajePedido);
    Logger::logMessage(Logger::TRACE, "Recibe pedido de fabricación");
    return mensajePedido.pedidoFabricacion;
}

void ControladorAlmacenPiezas::enviarPedidoProduccionARobot5(PedidoProduccion pedidoProduccion)
{
    MensajePedidoProduccion mensaje;
    mensaje.pedidoProduccion = pedidoProduccion;
    mensaje.mtype = TIPO_PEDIDO_PRODUCCION;
 
    sprintf(this->buffer, "Enviando pedido de Fabricación al Robot5: Cantidad:"
            "%d - Dif:%d - ODC:%d - Tipo:%d", pedidoProduccion.cantidad, 
            pedidoProduccion.diferenciaMinima, pedidoProduccion.nroOrdenCompra, 
            pedidoProduccion.tipo);
    Logger::logMessage(Logger::DEBUG, this->buffer);
    colaEnvioMensajePedidoProduccion.enviarPedidoProduccion(mensaje);
}

BufferCanastos ControladorAlmacenPiezas::obtenerBufferCanastos(int numBufferCanasto) {
    BufferCanastos canastos;
    
    this->semMemCanastos.wait(numBufferCanasto);
    this->shMemBufferCanastos[numBufferCanasto].readInfo(&canastos);
    this->semMemCanastos.signal(numBufferCanasto);
    return canastos;
}
/*
void ControladorAlmacenPiezas::avisarAAGVQueAgregueCanasto(TipoPieza tipoPieza, EspecifProd piezasReservadasTemporalmente[2]) {
    try {
        Logger::getInstance();
        sprintf(this->buffer, "Almacen piezas -:");
        Logger::setProcessInformation(this->buffer);
        BufferCanastos canastos;
        int posicionesPedidos[MAX_PIEZAS_POR_PRODUCTO];
        int cantPedidosRealizados;
        int posTemp;
        
        for (int i = 0; i < CANTIDAD_AGVS; i++) {
            cantPedidosRealizados = 0;
            this->semMemCanastos.wait(i);
            this->shMemBufferCanastos[i].readInfo(&canastos);
            this->semMemCanastos.signal(i);
            bool canastoPresente = false;
            
            // j recorre el buffer de los canastos
            int j = 0;
            while (!canastoPresente && j < MAX_QUANTITY_CANASTOS) {
                canastoPresente = canastos.canastos[j].tipoPieza == tipoPieza;
                j++;
            }
            if (canastoPresente)
                continue;
            // esta mal este buscar, no contempla los envios anteriores a los AGVs!!!!!!!!!!!!!!!
            
            for (int j = 0; j < MAX_QUANTITY_CANASTOS; j++) {
                // k es para comparar con ambos conjuntos de piezas utilizados anteriormente
                for (int k = 0; k < 2; k++) {
                    if (piezasReservadasTemporalmente[k].idProducto == NULL_PRODUCT)
                        continue;
                    // i = 1 ==> que estoy chequeando contra los robots 12, usan todas las piezas, los robots 11 solo usan la pantalla
                    if (i == 1) {
                        int l = 0;
                        while (!canastoPresente && l < piezasReservadasTemporalmente[k].cantPiezas) {
                            canastoPresente = canastos.canastos[j].tipoPieza == piezasReservadasTemporalmente[k].pieza[l].tipoPieza;
                            l++;
                        }
                    } else
                        canastoPresente = canastos.canastos[j].tipoPieza == piezasReservadasTemporalmente[k].tipoPantalla.tipoPieza;
                }
                //chequea que la posicion del canasto que voy a pedir no la haya pedido previamente
                posTemp = 0;
                
                
                sprintf(this->buffer, "antes del while!!!!!  pos a pedir %d, cant pedidos %d", j, cantPedidosRealizados);
                Logger::getInstance().logMessage(Logger::IMPORTANT, this->buffer);
                while(!canastoPresente && posTemp < cantPedidosRealizados) {
                    sprintf(this->buffer, "pos a pedir %d, pos pedida = %d", j, posicionesPedidos[posTemp]);
                    Logger::getInstance().logMessage(Logger::DEBUG, this->buffer);
                    
                    canastoPresente = j == posicionesPedidos[posTemp];
                    posTemp++;
                }
                if (!canastoPresente) {
                    MensajePedidoRobotCinta_6 mensaje;
                    mensaje.mtype = i + 1;
                    mensaje.pedidoCanastoAgv.lugar = j;
                    mensaje.pedidoCanastoAgv.tipoPieza = tipoPieza;
                    sprintf(this->buffer, "cambio el canasto del buffer %d que tiene la pieza %d por Pieza: %d en el lugar %d", i, canastos.canastos[j].tipoPieza, mensaje.pedidoCanastoAgv.tipoPieza, mensaje.pedidoCanastoAgv.lugar);
                    Logger::getInstance().logMessage(Logger::DEBUG, this->buffer);
                    this->colaPedidosCanastos.enviarPedidoCanasto(mensaje);
                    posicionesPedidos[cantPedidosRealizados] = j;
                    cantPedidosRealizados++;
                    break;
                }
            }
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}
*/
void ControladorAlmacenPiezas::avisarAAGVQueAgregueCanasto(int numAGV, 
        int posCanasto, TipoPieza tipoPieza) {
    
    MensajePedidoRobotCinta_6 mensaje;
    mensaje.mtype = numAGV + 1;
    mensaje.pedidoCanastoAgv.lugar = posCanasto;
    mensaje.pedidoCanastoAgv.tipoPieza = tipoPieza;
    colaPedidosCanastos.enviarPedidoCanasto(mensaje);
}

void ControladorAlmacenPiezas::recibirConfirmacionProduccion() {
    MensajeProximoPedidoProduccion mensajeProximoPedido;
    this->colaEnvioMensajePedidoProduccion.recibirProximoPedidoProduccion(TIPO_PEDIDO_ROBOT_5_ALMACEN_PIEZAS, &mensajeProximoPedido);
}