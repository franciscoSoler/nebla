#include "ControllerDespacho.h"
#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControllerDespacho::ControllerDespacho() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Despacho:");

        MiddlewareAPI middleware;
        middleware.crearCanales(1, ID_TIPO_DESPACHO);

        inputQueueDespacho_ = COMM::CommMsgHandler(1, ID_TIPO_DESPACHO);
        inputQueueDespacho_.setReceptorInfo("inputQueueDespacho", DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);

        inputQueueCliente_ = COMM::CommMsgHandler(1, ID_TIPO_DESPACHO, ID_TIPO_CLIENTE);
        inputQueueCliente_.setReceptorInfo("inputQueueCliente", DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);

        inputQueueR16_Despacho_ = COMM::CommMsgHandler(1, ID_TIPO_DESPACHO, ID_TIPO_ROBOT16_DESPACHO);
        inputQueueR16_Despacho_.setReceptorInfo("inputQueueR16_Despacho", DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);
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
        inputQueueDespacho_.recv(ID_DESPACHO, pedido);
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
        inputQueueR16_Despacho_.send( ID_R16_DESPACHO, msgPedido );
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
