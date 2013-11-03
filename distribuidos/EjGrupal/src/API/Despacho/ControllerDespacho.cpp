#include "ControllerDespacho.h"

ControllerDespacho::ControllerDespacho() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Despacho:");

        outputQueueDespacho_ = IPC::MsgQueue("outputQueueDespacho");
        outputQueueDespacho_.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_OUTPUT_ID);

        outputQueueCliente_ = IPC::MsgQueue("outputQueueCliente");
        outputQueueCliente_.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_OUTPUT_ID);

        outputQueueRobot16_ = IPC::MsgQueue("outputQueueRobot16");
        outputQueueRobot16_.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_ROBOT16_OUTPUT_ID);
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
        PedidoDespacho pedido;
        outputQueueDespacho_.recv(0, pedido);
        Logger::logMessage(Logger::TRACE, "Recibe un Pedido");
        return pedido;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

OrdenDeCompra ControllerDespacho::obtenerOrdenDeCompra(long idOrdenDeCompra) {
    try {
        PedidoOrdenDeCompra pedido;
        outputQueueDespacho_.recv(idOrdenDeCompra, pedido);
        sprintf(buffer_, "Recibe Orden de Compra N°%lu", idOrdenDeCompra);
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
        outputQueueRobot16_.send(pedido);
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
        
        outputQueueCliente_.send(pedido);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());   
        abort();
    }
}