/* 
 * File:   ControladorAlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 1:47 PM
 */

#include <sstream>

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

    // sprintf(this->buffer,"Num buffer canasto: %d",numBufferCanasto);
    // Logger::logMessage(Logger::ERROR, this->buffer);
    
    this->semMemCanastos.wait(numBufferCanasto);
    this->shMemBufferCanastos[numBufferCanasto].readInfo(&canastos);
    this->semMemCanastos.signal(numBufferCanasto);
    return canastos;
}

void ControladorAlmacenPiezas::avisarAAGVQueAgregueCanasto(int numAGV, 
        PedidoCanastoRobotCinta6 pedidoCanasto) {
    
    MensajePedidoRobotCinta_6 mensaje;
    mensaje.mtype = numAGV + 1;
    mensaje.pedidoCanastoAgv = pedidoCanasto;
    colaPedidosCanastos.enviarPedidoCanasto(mensaje);
}

void ControladorAlmacenPiezas::recibirConfirmacionProduccion() {
    MensajeProximoPedidoProduccion mensajeProximoPedido;
    this->colaEnvioMensajePedidoProduccion.recibirProximoPedidoProduccion(TIPO_PEDIDO_ROBOT_5_ALMACEN_PIEZAS, &mensajeProximoPedido);
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
