#include "ControllerDespacho.h"

ControllerDespacho::ControllerDespacho() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Despacho:");

        inputQueueDespacho_ = IPC::MsgQueue("inputQueueDespacho");
        inputQueueDespacho_.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);

        inputQueueCliente_ = IPC::MsgQueue("inputQueueCliente");
        inputQueueCliente_.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);

        inputQueueR16_Despacho_ = IPC::MsgQueue("inputQueueR16_Despacho");
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

OrdenDeCompra ControllerDespacho::obtenerOrdenDeCompra(long idOrdenDeCompra) {
    try {
        Msg_EnvioODCDespacho pedido;
        inputQueueDespacho_.recv(MSG_ENVIO_ODC_DESPACHO, pedido);
        sprintf(buffer_, "Recibe Orden de Compra N°%lu", pedido.ordenDeCompra_.idOrden_);
        Logger::logMessage(Logger::TRACE, buffer_);

        return pedido.ordenDeCompra_;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerDespacho::despacharProducto(TipoProducto idProducto, long idOrdenCompra) {
    try {
        Logger::logMessage(Logger::TRACE, "Notifica a Robot16 "
        "que debe sacar una caja del APT");
        PedidoDespacho pedido;
        pedido.idProducto_ = idProducto;
        pedido.idOrdenDeCompra_ = idOrdenCompra;

        Msg_FinProductoR16 msgPedido;
        msgPedido.mtype = MSG_FIN_PRODUCTO_R16;
        msgPedido.pedido_ = pedido;
        inputQueueR16_Despacho_.send( msgPedido );
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerDespacho::notificarAClienteProductoTerminado(long idCliente,
        long idOrdenCompra, TipoProducto idProducto) {
    try {
        sprintf(buffer_, "Notifica a Cliente N°%lu que puede retirar su pedido: "
        "idProducto: %d - idOrdenCompra: %lu.", idCliente, idProducto, idOrdenCompra);
        Logger::logMessage(Logger::IMPORTANT, buffer_);
        
        PedidoDespacho pedido;
        pedido.idCliente_ = idCliente;
        pedido.idOrdenDeCompra_ = idOrdenCompra;
        pedido.idProducto_ = idProducto;

        Msg_RetiroProducto msgACliente;
        msgACliente.mtype = MSG_RETIRO_PRODUCTO;
        msgACliente.datos_ = pedido;
        
        inputQueueCliente_.send( msgACliente );
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());   
        abort();
    }
}
