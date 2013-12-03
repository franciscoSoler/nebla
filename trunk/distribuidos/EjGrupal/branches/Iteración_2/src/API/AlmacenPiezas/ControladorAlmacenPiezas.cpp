/* 
 * File:   ControladorAlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 1:47 PM
 */

#include <sstream>

#include <middlewareCommon.h>
#include <Comunicaciones/Objects/MiddlewareAPI.h>
#include "ControladorAlmacenPiezas.h"
#include "../../Logger/Logger.h"
#include "LockFile.h"

ControladorAlmacenPiezas::ControladorAlmacenPiezas() //:
        //colaEnvioMensajePedidoProduccion ("Mensaje Robot 5 Msg Queue", 1, ID_TIPO_AP, ID_TIPO_ROBOT5_CINTA)
{
    try {
        Logger::setProcessInformation("AlmacenDePiezas:");

        MiddlewareAPI middleware;
        middleware.crearCanales(1, ID_TIPO_AP);

        this->colaReciboOrdenProduccion = IPC::MsgQueue("ColaReciboOP", 1, ID_TIPO_AP, ID_TIPO_VENDEDOR);
        this->colaReciboOrdenProduccion.getMsgQueue(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);

        this->colaEnvioMensajePedidoProduccion =
        IPC::PedidosProduccionMessageQueue("ColaEnvioMensajePedido", 1, ID_TIPO_AP, ID_TIPO_ROBOT5_CINTA);
        colaEnvioMensajePedidoProduccion.getMessageQueue(DIRECTORY_ROBOT_5, ID_COLA_PEDIDOS_PRODUCCION);

        this->colaPedidosCanastos = IPC::PedidosCanastosMessageQueue("colaPedidosCanastos", 1, ID_TIPO_AP, ID_TIPO_AGV);
        this->colaPedidosCanastos.getMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);

        this->shMemBufferCanastos[CANTIDAD_AGVS] = IPC::BufferCanastosSharedMemory("shMemBufferCanastos");
        this->shMemBufferCanastos[0].getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_0);
        this->shMemBufferCanastos[1].getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_1);
        this->shMemBufferCanastos[2].getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_2);

        this->semMemCanastos = IPC::Semaphore("semMemCanastos");
        this->semMemCanastos.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, 3);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

ControladorAlmacenPiezas::~ControladorAlmacenPiezas() { }

pedido_fabricacion_t ControladorAlmacenPiezas::recibirPedidoDeFabricacion()
{
    try {
        mensaje_pedido_fabricacion_t mensajePedido;
        this->colaReciboOrdenProduccion.recv(1, mensajePedido);
        return mensajePedido.pedidoFabricacion;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorAlmacenPiezas::enviarPedidoProduccionARobot5(PedidoProduccion pedidoProduccion)
{
    try {
        MensajePedidoProduccion mensaje;
        mensaje.pedidoProduccion = pedidoProduccion;

        sprintf(this->buffer, "Enviando pedido de Fabricación al Robot5: Cantidad:"
                "%d - Dif:%d - ODC:%d - Tipo:%d", pedidoProduccion.cantidad,
                pedidoProduccion.diferenciaMinima, pedidoProduccion.nroOrdenCompra,
                pedidoProduccion.tipo);
        Logger::logMessage(Logger::DEBUG, this->buffer);
        colaEnvioMensajePedidoProduccion.enviarPedidoProduccion(TIPO_PEDIDO_PRODUCCION, mensaje);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

BufferCanastos ControladorAlmacenPiezas::obtenerBufferCanastos(int numBufferCanasto) {
    BufferCanastos canastos;
    
    try {
        this->semMemCanastos.wait(numBufferCanasto);
        this->shMemBufferCanastos[numBufferCanasto].readInfo(&canastos);
        this->semMemCanastos.signal(numBufferCanasto);
        return canastos;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorAlmacenPiezas::avisarAAGVQueAgregueCanasto(int numAGV, 
        PedidoCanastoRobotCinta6 pedidoCanasto) {
    
    try {
        MensajePedidoRobotCinta_6 mensaje;
        mensaje.pedidoCanastoAgv = pedidoCanasto;
        colaPedidosCanastos.enviarPedidoCanasto(numAGV + 1, mensaje);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorAlmacenPiezas::recibirConfirmacionProduccion() {
    try {
        MensajeProximoPedidoProduccion mensajeProximoPedido;
        this->colaEnvioMensajePedidoProduccion.recibirProximoPedidoProduccion(
                    ID_ALMACEN_PIEZAS, &mensajeProximoPedido);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorAlmacenPiezas::imprimirEspecificacionProducto(EspecifProd especifProd) {
    std::stringstream ss;
    ss << "Tipo pantalla:" << especifProd.tipoPantalla.tipoPieza << std::endl;
    ss << "Cantidad de Piezas: " << especifProd.cantPiezas << std::endl;
    for (int i = 0; i < especifProd.cantPiezas-1; ++i) {
        ss << "Tipo Pieza: " << especifProd.pieza[i].tipoPieza << 
        " - Cantidad: " << especifProd.pieza[i].cantidad << std::endl;     
    }
    Logger::logMessage(Logger::IMPORTANT, ss.str());
}
