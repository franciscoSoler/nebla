#include "ControllerRobot16.h"
#include <Exceptions/Exception.h>
#include <Logger/Logger.h>

ControllerRobot16::ControllerRobot16() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Robot16:");
        shMem_R14_R16_Data_ = new DataSM_R14_R16();
             
        // Se crean los IPCs
        shMem_R14_R16_ = IPC::SharedMemory<DataSM_R14_R16>("shMem_R14_R16");
        shMem_R14_R16_.getSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
        
        semMutex_shMem_R14_R16_ = IPC::Semaphore ("semMutex_shMem_R14_R16");
        semMutex_shMem_R14_R16_.getSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
        
        semR14_ = IPC::Semaphore("semR14");
        semR14_.getSemaphore(DIRECTORY_ROBOT_14, SEM_R14_ID, 1);
        
        semR16_ = IPC::Semaphore("semR16");
        semR16_.getSemaphore(DIRECTORY_ROBOT_16, SEM_R16_ID, 1);
        
        outputQueueR16_ = IPC::MsgQueue("outputQueueR16");
        outputQueueR16_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_ROBOT16_OUTPUT_ID);
        
        R16_Cliente_Queue_ = IPC::MsgQueue("R16_Cliente_Queue");
        R16_Cliente_Queue_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);
        
        outputQueueDespacho_ = IPC::MsgQueue("outputQueueDespacho");
        outputQueueDespacho_.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_OUTPUT_ID);
        
        semMutex_shMem_APT = IPC::Semaphore("semMutex_APT");
        semMutex_shMem_APT.getSemaphore(DIRECTORY_APT, SEM_MUTEX_SM_APT_ID, 1);
        
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }
}

PedidoDespacho ControllerRobot16::recibirPedido() {
    try {
        Logger::setProcessInformation("Robot16 - recibirPedido:");
        outputQueueR16_.recv(0, pedido_);
        
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

bool ControllerRobot16::moverCinta() {
    try {
        Logger::setProcessInformation("Robot16 - moverCinta:");
        bool cintaVacia = true;
        bool cintaMovida = false;
        
        while (cintaVacia) {
            // Intenta mover la cinta
            obtener_shMem_R14_R16();

            if (shMem_R14_R16_Data_->getCantidadElementosEnCinta() == 0) {
                Logger::logMessage(Logger::TRACE, "Cinta vacía. Robot se bloquea");
                shMem_R14_R16_Data_->setEstadoBloqueoRobot16(true);
                liberar_shMem_R14_R16();
                
                semR16_.wait();
                Logger::logMessage(Logger::TRACE, "Es despertado");
            }
            else {
                cintaMovida = avanzarCinta();
                liberar_shMem_R14_R16();
                cintaVacia = false;
            }
        }
        
        return cintaMovida;
    } 
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot16::tomarCajaCinta15(Caja & unaCaja) {
    try {
        Logger::setProcessInformation("Robot16 - tomarCaja:");
        obtener_shMem_R14_R16();
        bool retVal = shMem_R14_R16_Data_->retirarCajaDeCinta(unaCaja);

        if (retVal) {
            Logger::logMessage(Logger::TRACE, "Retira caja de la Cinta15");

            // El robot14 ahora no está trabajando sobre ninguna cinta, dado que 
            // está intentando depositar la misma en la Cinta15
            shMem_R14_R16_Data_->setEstadoTrabajoRobot16(false);
        }
        else {
            Logger::logMessage(Logger::TRACE, "No se pudo retirar caja de la Cinta15");
        }

        liberar_shMem_R14_R16();
        return retVal;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot16::depositarCajaEnAPT(Caja unaCaja, long & idNroOrdenAPT) {
    try {
        Logger::setProcessInformation("Robot16 - depositarCaja:");
        semMutex_shMem_APT.wait();
        
        Logger::logMessage(Logger::TRACE, "Deposita Caja en APT");
        bool resultado = shMem_APT_.depositarCaja( unaCaja );
        semMutex_shMem_APT.signal();

        obtener_shMem_R14_R16();
        shMem_R14_R16_Data_->setEstadoTrabajoRobot16(false);
        liberar_shMem_R14_R16();
        
        return resultado;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerRobot16::tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja) {
    try {
        Logger::setProcessInformation("Robot16 - tomarCajaDeAPT:");
        semMutex_shMem_APT.wait();
        shMem_APT_.sacarCaja(unaCaja, pedido.idProducto_, pedido.idOrdenDeCompra_);
        Logger::logMessage(Logger::TRACE, "Saca Caja del APT");
        semMutex_shMem_APT.signal();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();        
    }

} 

void ControllerRobot16::informarAlDespachoProductoTerminado(long idNroOrden, 
                                                            TipoProducto tipo) {
    try {
        PedidoDespacho pedido;
        pedido.tipoPedido_ = PEDIDO_ROBOT16;
        pedido.idOrdenDeCompra_ = idNroOrden;
        pedido.idProducto_ = tipo;

        sprintf(buffer_, "Robot informa que el tipo de producto %u de la ODC "
                "N°%lu ha sido terminado", tipo, idNroOrden);
        Logger::logMessage(Logger::TRACE, buffer_);

        outputQueueDespacho_.send(pedido);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();                
    }
}

void ControllerRobot16::enviarCajaAlCliente(long idCliente, Caja caja) {
    try {
        Logger::logMessage(Logger::TRACE, "Se envía Caja al cliente");
        EnvioCajaCliente envio;
        envio.mtype = idCliente;
        envio.caja = caja;
        R16_Cliente_Queue_.send(envio);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();                
    }
}

ControllerRobot16::~ControllerRobot16() {
    delete shMem_R14_R16_Data_;
}

void ControllerRobot16::obtener_shMem_R14_R16() {
    semMutex_shMem_R14_R16_.wait();
    // Logger::logMessage(Logger::TRACE, "Accede a memoria compartida R11-R14");
    shMem_R14_R16_.read(shMem_R14_R16_Data_);
}

void ControllerRobot16::liberar_shMem_R14_R16() {
    shMem_R14_R16_.write(shMem_R14_R16_Data_);
    // Logger::logMessage(Logger::TRACE, "Libera memoria compartida R11-R14");
    semMutex_shMem_R14_R16_.signal();
}

bool ControllerRobot16::avanzarCinta() {
    bool cintaMovida = shMem_R14_R16_Data_->moverCinta();
    if (cintaMovida) {

        sprintf(buffer_, "Cinta15 es movida. %s", 
                shMem_R14_R16_Data_->cintaToString().c_str()); 
        Logger::logMessage(Logger::DEBUG, buffer_);

        // Se intenta despertar al Robot 14 si es que este está dormido
        if (shMem_R14_R16_Data_->estaRobot14Bloqueado()) {
            Logger::logMessage(Logger::TRACE, "Se despierta al Robot14 de la Cinta15");

            shMem_R14_R16_Data_->setEstadoBloqueoRobot14(false);
            semR14_.signal();
        }
    }
    else {
        Logger::logMessage(Logger::TRACE, "Cinta15 no pudo ser movida.");        
    }

    liberar_shMem_R14_R16();
    return cintaMovida;
}

