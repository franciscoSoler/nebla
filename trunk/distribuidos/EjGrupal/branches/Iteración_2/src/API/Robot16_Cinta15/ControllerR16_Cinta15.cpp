#include "ControllerR16_Cinta15.h"
#include <Exceptions/Exception.h>
#include <Logger/Logger.h>
#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControllerR16_Cinta15::ControllerR16_Cinta15() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Robot16_Cinta15:");
        shMem_R14_R16_Data_ = new DataSM_R14_R16();


        MiddlewareAPI middleware;
        middleware.crearCanales(1, ID_TIPO_ROBOT16_CINTA);
             
        int idEmisor = 1;
        
        // Se crean los IPCs
        shMem_R14_R16_ = IPC::SharedMemory<SerializedData>("shMem_R14_R16");
        shMem_R14_R16_.getSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
        
        shmemAlmacenTerminados = IPC::SharedMemory<AlmacenProductosTerminados>("shMemAlmacenTerminados");
        shmemAlmacenTerminados.getSharedMemory(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS);
        
        semMutex_shMem_R14_R16_ = COMM::CommSemaphoreMutex<SerializedData> ("semMutex_shMem_R14_R16", idEmisor, ID_TIPO_ROBOT16_CINTA);
        semMutex_shMem_R14_R16_.getSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
        semMutex_shMem_R14_R16_.setShMem(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
        
        semR14_Cinta15_ = COMM::CommSemaphore("semR14_Cinta15", idEmisor, ID_TIPO_ROBOT16_CINTA, ID_TIPO_ROBOT14);
        semR14_Cinta15_.getSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA15_ID, 1);
        
        inputQueueR16_Cinta15_ = COMM::CommMsgHandler(idEmisor, ID_TIPO_ROBOT16_CINTA, ID_TIPO_ROBOT14);
        inputQueueR16_Cinta15_.setReceptorInfo("inputQueueR16_Cinta15",
                                               DIRECTORY_ROBOT_16, MSGQUEUE_R16_CINTA15_INPUT_ID);
        
        inputQueueDespacho_ = COMM::CommMsgHandler(idEmisor, ID_TIPO_ROBOT16_CINTA, ID_TIPO_DESPACHO);
        inputQueueDespacho_.setReceptorInfo("inputQueueDespacho",
                                            DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);
        
        semMutex_shMem_APT_ = COMM::CommSemaphoreMutex<AlmacenProductosTerminados>("semMutex_shMem_APT", idEmisor, ID_TIPO_ROBOT16_CINTA);
        semMutex_shMem_APT_.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
        semMutex_shMem_APT_.setShMem(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS);

        semMutex_sincronismo_R16_ = IPC::Semaphore("semMutex_sincronismo_R16");
        semMutex_sincronismo_R16_.getSemaphore(DIRECTORY_ROBOT_16, SEM_MUTEX_SINCRONISMO_R16_ID, 1);
        
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }
}

void ControllerR16_Cinta15::recibirOrdenParaMoverCaja() {
    try {
        Logger::setProcessInformation("Robot16_Cinta15 - recibirPedido:");
        inputQueueR16_Cinta15_.recv(MSG_AVISO_CAJA_CINTA_15, mensaje_);
        Logger::logMessage(Logger::TRACE, "Recibe Pedido");
        
        obtener_shMem_R14_R16();
        shMem_R14_R16_Data_->setEstadoTrabajoRobot16(true);
        liberar_shMem_R14_R16();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerR16_Cinta15::moverCinta() {
    try {
        Logger::setProcessInformation("Robot16_Cinta15 - moverCinta:");
        bool cintaMovida = false;

        obtenerMutexSincronismo();
        // Si llegó un mensaje es porque hay una caja en la cinta, intenta mover la misma
        obtener_shMem_R14_R16();

        cintaMovida = avanzarCinta();
        liberar_shMem_R14_R16();
        liberarMutexSincronismo();
        return cintaMovida;
    } 
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerR16_Cinta15::tomarCajaCinta15(Caja & unaCaja) {
    try {
        Logger::setProcessInformation("Robot16_Cinta15 - tomarCaja:");
        obtenerMutexSincronismo();
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

        liberarMutexSincronismo();
        liberar_shMem_R14_R16();
        return retVal;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerR16_Cinta15::depositarCajaEnAPT(Caja unaCaja, long & idNroOrdenAPT) {
    try {
        Logger::setProcessInformation("Robot16_Cinta15 - depositarCaja:");
        obtenerMutexSincronismo();
        semMutex_shMem_APT_.wait();
        
        AlmacenProductosTerminados almacenPT;
        shmemAlmacenTerminados.read(&almacenPT);
        
        sprintf(buffer_, "Deposita Caja en APT - TipoProducto: %d, idNroOrden: %ld", unaCaja.idProducto_, idNroOrdenAPT);
        Logger::logMessage(Logger::TRACE, buffer_);
        
        shMem_APT_.establecerMatriz(almacenPT.almacen);
        bool resultado = shMem_APT_.depositarCaja(unaCaja, idNroOrdenAPT);
        
        shMem_APT_.obtenerMatriz(almacenPT.almacen);
        shmemAlmacenTerminados.write(&almacenPT);
        semMutex_shMem_APT_.signal();
        
        //sprintf(buffer_, "Deposito Caja en APT - TipoProducto: %d, idNroOrdenVieja: %ld idNroOrdenNueva: %ld", unaCaja.idProducto_, unaCaja.idOrdenDeCompra_, idNroOrdenAPT);
        //Logger::logMessage(Logger::TRACE, buffer_);

        obtener_shMem_R14_R16();
        shMem_R14_R16_Data_->setEstadoTrabajoRobot16(false);
        liberar_shMem_R14_R16();
        
        liberarMutexSincronismo();
        return resultado;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerR16_Cinta15::informarAlDespachoProductoTerminado(long idNroOrden, 
                                                            TipoProducto tipo) {
    try {
        PedidoDespacho pedido;
        pedido.tipoPedido_ = PEDIDO_ROBOT16;
        pedido.idOrdenDeCompra_ = idNroOrden;
        pedido.idProducto_ = tipo;

        Msg_PedidoDespacho mensaje;
        mensaje.pedido_ = pedido;

        obtenerMutexSincronismo();
        sprintf(buffer_, "Robot informa que el tipo de producto %u de la ODC "
                "N°%lu ha sido terminado", tipo, idNroOrden);
        Logger::logMessage(Logger::TRACE, buffer_);

        inputQueueDespacho_.send(ID_DESPACHO, mensaje);
        liberarMutexSincronismo();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();                
    }
}


ControllerR16_Cinta15::~ControllerR16_Cinta15() {
    delete shMem_R14_R16_Data_;
}

void ControllerR16_Cinta15::obtener_shMem_R14_R16() {
    SerializedData buffer;

    semMutex_shMem_R14_R16_.wait();
    // Logger::logMessage(Logger::TRACE, "Accede a memoria compartida R14-R16");
    shMem_R14_R16_.read(&buffer);

    shMem_R14_R16_Data_->deserializeData(buffer);
}

void ControllerR16_Cinta15::liberar_shMem_R14_R16() {
    SerializedData buffer;
    buffer = this->shMem_R14_R16_Data_->serializeData();

    shMem_R14_R16_.write( &buffer );
    // Logger::logMessage(Logger::TRACE, "Libera memoria compartida R14-R16");
    semMutex_shMem_R14_R16_.signal();
}

bool ControllerR16_Cinta15::avanzarCinta() {
    bool cintaMovida = shMem_R14_R16_Data_->moverCinta();
    if (cintaMovida) {

        sprintf(buffer_, "Cinta15 es movida. %s", 
                shMem_R14_R16_Data_->cintaToString().c_str()); 
        Logger::logMessage(Logger::DEBUG, buffer_);

        // Se intenta despertar al Robot 14 si es que este está dormido
        if (shMem_R14_R16_Data_->estaRobot14Bloqueado()) {
            Logger::logMessage(Logger::TRACE, "Se despierta al Robot14 de la Cinta15");

            shMem_R14_R16_Data_->setEstadoBloqueoRobot14(false);
            semR14_Cinta15_.signal();
        }
    }
    else {
        Logger::logMessage(Logger::TRACE, "Cinta15 no pudo ser movida.");        
    }

    return cintaMovida;
}

void ControllerR16_Cinta15::obtenerMutexSincronismo() {
    semMutex_sincronismo_R16_.wait();
    // Logger::logMessage(Logger::IMPORTANT, "Obtiene control del Robot16");
}

void ControllerR16_Cinta15::liberarMutexSincronismo() {
    // Logger::logMessage(Logger::IMPORTANT, "Libera control del Robot16");
    semMutex_sincronismo_R16_.signal();
}

