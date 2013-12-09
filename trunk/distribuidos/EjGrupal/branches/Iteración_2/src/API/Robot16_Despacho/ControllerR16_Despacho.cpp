#include "ControllerR16_Despacho.h"
#include <Exceptions/Exception.h>
#include <Logger/Logger.h>
#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControllerR16_Despacho::ControllerR16_Despacho() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Robot16_Despacho:");
        shMem_R14_R16_Data_ = new DataSM_R14_R16();

        MiddlewareAPI middleware;
        middleware.crearCanales(1, ID_TIPO_ROBOT16_DESPACHO);
             
        // Se crean los IPCs
        shMem_R14_R16_ = IPC::SharedMemory<DataSM_R14_R16>("shMem_R14_R16");
        shMem_R14_R16_.getSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
        
        shmemAlmacenTerminados = IPC::SharedMemory<AlmacenProductosTerminados>("shMemAlmacenTerminados");
        shmemAlmacenTerminados.getSharedMemory(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS);
        
        semMutex_shMem_R14_R16_ = COMM::CommSemaphoreMutex<DataSM_R14_R16> ("semMutex_shMem_R14_R16");
        semMutex_shMem_R14_R16_.getSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
        semMutex_shMem_R14_R16_.setShMem(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
        
        inputQueueR16_Despacho_ = COMM::CommMsgHandler(1, ID_TIPO_ROBOT16_DESPACHO, ID_TIPO_DESPACHO);
        inputQueueR16_Despacho_.setReceptorInfo("inputQueueR16_Despacho",
                                                DIRECTORY_ROBOT_16, MSGQUEUE_R16_DESPACHO_INPUT_ID);
        
        R16_Cliente_Queue_ = COMM::CommMsgHandler(1, ID_TIPO_ROBOT16_DESPACHO, ID_TIPO_CLIENTE);
        R16_Cliente_Queue_.setReceptorInfo("R16_Cliente_Queue",
                                           DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);

        semMutex_shMem_APT_ = COMM::CommSemaphoreMutex<AlmacenProductosTerminados>("semMutex_shMem_APT");
        semMutex_shMem_APT_.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
        semMutex_shMem_APT_.setShMem(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }
}

PedidoDespacho ControllerR16_Despacho::recibirPedido(bool & ultimoProductoDeODC) {
    PedidoDespacho pedido_;
    try {
        Logger::setProcessInformation("Robot16_Despacho - recibirPedido:");
        inputQueueR16_Despacho_.recv(ID_R16_DESPACHO, mensaje_);

        pedido_ = mensaje_.pedido_;
        ultimoProductoDeODC = mensaje_.ultimoProductoDeODC_;
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

    return pedido_;
}

void ControllerR16_Despacho::tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja) {
    try {
        Logger::setProcessInformation("Robot16_Despacho - tomarCajaDeAPT:");
        semMutex_shMem_APT_.wait();
        
        AlmacenProductosTerminados almacenPT;
        shmemAlmacenTerminados.read(&almacenPT);
        shMem_APT_.establecerMatriz(almacenPT.almacen);
        
        shMem_APT_.sacarCaja(unaCaja, pedido.idProducto_, pedido.idOrdenDeCompra_);
        Logger::logMessage(Logger::TRACE, "Saca Caja del APT");
        
        shMem_APT_.obtenerMatriz(almacenPT.almacen);
        shmemAlmacenTerminados.write(&almacenPT);
        
        semMutex_shMem_APT_.signal();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();        
    }

} 

void ControllerR16_Despacho::enviarCajaAlCliente(long idCliente, Caja caja, bool ultimoProductoDeODC) {
    try {
        Logger::setProcessInformation("Robot16_Despacho - enviarCajaAlCliente:");
        sprintf(buffer_, "Se envía Caja al cliente %ld", idCliente);
        Logger::logMessage(Logger::IMPORTANT, buffer_);

        Msg_EnvioCajaCliente mensaje;
        mensaje.caja = caja;
        mensaje.ultimoProductoDeODC_ = ultimoProductoDeODC;
        R16_Cliente_Queue_.send(idCliente, mensaje);
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


