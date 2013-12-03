#include "ControllerDespacho.h"
#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControllerDespacho::ControllerDespacho() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Despacho:");

        MiddlewareAPI middleware;
        middleware.crearCanales(1, ID_TIPO_DESPACHO);

        inputQueueDespacho_ = IPC::MsgQueue("inputQueueDespacho", 1, ID_TIPO_DESPACHO);
        inputQueueDespacho_.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);

        inputQueueCliente_ = IPC::MsgQueue("inputQueueCliente", 1, ID_TIPO_CLIENTE, ID_TIPO_DESPACHO);
        inputQueueCliente_.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);

        inputQueueR16_Despacho_ = IPC::MsgQueue("inputQueueR16_Despacho", 1, ID_TIPO_ROBOT16_DESPACHO, ID_TIPO_DESPACHO);
        inputQueueR16_Despacho_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

ControllerDespacho::~ControllerDespacho() {
}

PedidoDespacho ControllerDespacho::recibirPedido() {
    try {
        Msg_PedidoDespacho pedido;
        inputQueueDespacho_.recv(MSG_PEDIDO_DESPACHO, pedido);
        Logger::logMessage(Logger::TRACE, "Recibe un Pedido");
        return pedido.pedido_;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerDespacho::despacharProducto(PedidoDespacho pedido, bool ultimoProductoDeODC) {
    try {
        Logger::logMessage(Logger::TRACE, "Notifica a Robot16 "
        "que debe sacar una caja del APT");

        Msg_FinProductoR16 msgPedido;
        msgPedido.pedido_ = pedido;
        msgPedido.ultimoProductoDeODC_ = ultimoProductoDeODC;
        inputQueueR16_Despacho_.send( MSG_FIN_PRODUCTO_R16, msgPedido );
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerDespacho::notificarAClienteProductoTerminado(
        PedidoDespacho pedido, bool ultimoPedido) {
    try {
        Msg_RetiroProducto msgACliente;
        msgACliente.datos_ = pedido;
        msgACliente.ultimoPedido_ = ultimoPedido;
        
        inputQueueCliente_.send(pedido.idCliente_, msgACliente );
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());   
        abort();
    }
}
