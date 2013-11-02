/* 
 * File:   ControladorRobot5.cpp
 * Author: nebla
 * 
 * Created on October 22, 2013, 2:46 PM
 */

#include "ControladorRobot5.h"

ControladorRobot5::ControladorRobot5() {
}

ControladorRobot5::~ControladorRobot5() {
}

void ControladorRobot5::iniciarControlador() {
    /* Obtengo la cola de pedidos */
    comunicacionMsgQueue = ComunicacionRobot5MessageQueue();
    comunicacionMsgQueue.getMessageQueue(DIRECTORY,ID_COLA_API_ROBOT_5);
}

PedidoRobot5 ControladorRobot5::obtenerPedido() {
    MensajePedidoRobot5 mensajePedido;
    comunicacionMsgQueue.recibirPedidoRobot5(TIPO_PEDIDO_ROBOT_5, &mensajePedido);
    return mensajePedido.pedidoRobot5;
}

void ControladorRobot5::resolverPedido(Canasto canasto, int idDestino) {
    MensajeRespuestaCanasto mensajeRespuesta;
    mensajeRespuesta.idAgv = idDestino;
    mensajeRespuesta.canasto = canasto;
    mensajeRespuesta.mtype = TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5;
    comunicacionMsgQueue.enviarCanasto(mensajeRespuesta);
}

void ControladorRobot5::resolverPedido(Gabinete gabinete, bool ultimo, int ordenCompra) {
    MensajeRespuestaGabinete mensajeRespuesta;
    mensajeRespuesta.gabinete = gabinete;
    mensajeRespuesta.ultimo = ultimo;
    mensajeRespuesta.ordenDeCompra = ordenCompra;
    mensajeRespuesta.mtype = TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5;
    comunicacionMsgQueue.enviarGabinete(mensajeRespuesta);
}

Canasto ControladorRobot5::obtenerCanasto(TipoPieza tipoPieza) {
    Canasto canasto;
    canasto.tipoPieza = tipoPieza;
    canasto.cantidadPiezas = 10;
    return canasto;
}

Gabinete ControladorRobot5::obtenerGabinete(TipoProducto tipoPorudcto) {
    Gabinete gabinete;
    return gabinete;
}
