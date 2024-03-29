#include "ControllerR16_Despacho.h"
#include <Exceptions/Exception.h>
#include <Logger/Logger.h>

ControllerR16_Despacho::ControllerR16_Despacho() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Robot16_Despacho:");
        shMem_R14_R16_Data_ = new DataSM_R14_R16();
             
        // Se crean los IPCs
        shMem_R14_R16_ = IPC::SharedMemory<DataSM_R14_R16>("shMem_R14_R16");
        shMem_R14_R16_.getSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
        
        semMutex_shMem_R14_R16_ = IPC::Semaphore ("semMutex_shMem_R14_R16");
        semMutex_shMem_R14_R16_.getSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
        
        inputQueueR16_Despacho_ = IPC::MsgQueue("inputQueueR16_Despacho");
        inputQueueR16_Despacho_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);
        
        R16_Cliente_Queue_ = IPC::MsgQueue("R16_Cliente_Queue");
        R16_Cliente_Queue_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);
        
        inputQueueDespacho_ = IPC::MsgQueue("inputQueueDespacho");
        inputQueueDespacho_.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);
        
        semMutex_shMem_APT_ = IPC::Semaphore("semMutex_shMem_APT");
        semMutex_shMem_APT_.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }
}

PedidoDespacho ControllerR16_Despacho::recibirPedido() {
    try {
        Logger::setProcessInformation("Robot16_Despacho - recibirPedido:");
        inputQueueR16_Despacho_.recv(MSG_FIN_PRODUCTO_R16, mensaje_ );

        PedidoDespacho pedido_ = mensaje_.pedido_;
        obtener_shMem_R14_R16();

        if (pedido_.tipoPedido_ == PEDIDO_ROBOT16 && pedido_.idProducto_ == NULL_PRODUCT) {
            shMem_R14_R16_Data_->setEstadoTrabajoRobot16(true);
        }
        liberar_shMem_R14_R16();
        
        return pedido_;

        Logger::logMessage(Logger::TRACE, "Recibe Pedido");
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerR16_Despacho::tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja) {
    try {
        Logger::setProcessInformation("Robot16_Despacho - tomarCajaDeAPT:");
        semMutex_shMem_APT_.wait();
        shMem_APT_.sacarCaja(unaCaja, pedido.idProducto_, pedido.idOrdenDeCompra_);
        Logger::logMessage(Logger::TRACE, "Saca Caja del APT");
        semMutex_shMem_APT_.signal();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();        
    }

} 

void ControllerR16_Despacho::enviarCajaAlCliente(long idCliente, Caja caja) {
    try {
        Logger::setProcessInformation("Robot16_Despacho - enviarCajaAlCliente:");
        sprintf(buffer_, "Se envía Caja al cliente %ld", idCliente);
        Logger::logMessage(Logger::IMPORTANT, buffer_);

        Msg_EnvioCajaCliente mensaje;
        mensaje.mtype = idCliente;
        mensaje.caja = caja;
        R16_Cliente_Queue_.send(mensaje);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();                
    }
}

ControllerR16_Despacho::~ControllerR16_Despacho() {
    delete shMem_R14_R16_Data_;
}

void ControllerR16_Despacho::obtener_shMem_R14_R16() {
    semMutex_shMem_R14_R16_.wait();
    // Logger::logMessage(Logger::TRACE, "Accede a memoria compartida R11-R14");
    shMem_R14_R16_.read(shMem_R14_R16_Data_);
}

void ControllerR16_Despacho::liberar_shMem_R14_R16() {
    shMem_R14_R16_.write(shMem_R14_R16_Data_);
    // Logger::logMessage(Logger::TRACE, "Libera memoria compartida R11-R14");
    semMutex_shMem_R14_R16_.signal();
}


