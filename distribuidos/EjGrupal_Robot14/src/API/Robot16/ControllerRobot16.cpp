#include "ControllerRobot16.h"
#include <CSO/Exceptions/Exception.h>
#include <CSO/Logger/Logger.h>

ControllerRobot16::ControllerRobot16() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Robot16:");
        shMem_R14_R16_Data_ = new DataSM_R14_R16();
             
        // Se crean los IPCs
        shMem_R14_R16_ = IPC::SharedMemory<DataSM_R14_R16>("ShMem_R14_R16");
        shMem_R14_R16_.getSharedMemory(DIRECTORY, SM_R14_R16_ID);
        
        semMutex_shMem_R14_R16_ = IPC::Semaphore ("semMutex_shMem_R14_R16");
        semMutex_shMem_R14_R16_.getSemaphore(DIRECTORY, SEM_MUTEX_SM_R14_R16_ID, 1);
        
        semR14_ = IPC::Semaphore("semR14");
        semR14_.getSemaphore(DIRECTORY, SEM_R14_ID, 1);
        
        semR16_ = IPC::Semaphore("semR16");
        semR16_.getSemaphore(DIRECTORY, SEM_R16_ID, 1);
        
        // Creo los estados
        estadoDurmiendo_ = new StateR16_Durmiendo(this);
        estadoTrabajando_ = new StateR16_Trabajando(this);
        estadoConCarga_ = new StateR16_ConCarga(this);
        estadoActual_ = estadoDurmiendo_;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }
}

bool ControllerRobot16::moverCinta() {
    try {
        Logger::setProcessInformation("Robot16 - moverCinta:");
        return estadoActual_->moverCinta();
    } 
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot16::tomarCaja(Caja & unaCaja) {
    try {
        Logger::setProcessInformation("Robot16 - tomarCaja:");
        return estadoActual_->tomarCaja(unaCaja);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerRobot16::depositarCaja(Caja unaCaja) {
    try {
        Logger::setProcessInformation("Robot16 - depositarCaja:");
        estadoActual_->depositarCaja(unaCaja);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot16::moverCintaEnEstadoDurmiendo() {
    Logger::logMessage(Logger::TRACE, "Se bloquea esperando que un Robot14 coloque cajas");
    semR16_.wait();
    Logger::logMessage(Logger::TRACE, "Es liberado");
    
    // Setea su estado trabajando en true y procede  a mover la cinta
    obtener_shMem_R14_R16();
    shMem_R14_R16_Data_->setEstadoTrabajoRobot16 (true); 
    setEstado( getEstadoTrabajando() );

    bool cintaMovida = avanzarCinta();
    liberar_shMem_R14_R16();
    return cintaMovida;
}

bool ControllerRobot16::moverCintaEnEstadoTrabajando() {
    obtener_shMem_R14_R16();
    bool cintaMovida = avanzarCinta();
    liberar_shMem_R14_R16();
    
    return cintaMovida;
}

bool ControllerRobot16::tomarCajaEnEstadoTrabajando(Caja & unaCaja) {
    obtener_shMem_R14_R16();
    bool retVal = shMem_R14_R16_Data_->retirarCajaDeCinta(unaCaja);
    
    if (retVal) {
        Logger::logMessage(Logger::TRACE, "Retira caja de la Cinta15");

        // El robot14 ahora no está trabajando sobre ninguna cinta, dado que 
        // está intentando depositar la misma en la Cinta15
        shMem_R14_R16_Data_->setEstadoTrabajoRobot16(false);
        setEstado( getEstadoConCarga() );
    }
    else {
        Logger::logMessage(Logger::TRACE, "No se pudo retirar caja de la Cinta15");
    }

    liberar_shMem_R14_R16();
    return retVal;
}

void ControllerRobot16::depositarCajaEnEstadoConCarga(Caja & caja) {
    depositarCajaEnAPT(caja);
    
    obtener_shMem_R14_R16();    
    if (! shMem_R14_R16_Data_->getCantidadElementosEnCinta() ) {
        setEstado( getEstadoDurmiendo() );
        shMem_R14_R16_Data_->setEstadoBloqueoRobot16(true);
    }
    else {
        setEstado( getEstadoTrabajando() );
    }
    
    liberar_shMem_R14_R16();
}

IStateR16* ControllerRobot16::getEstadoDurmiendo() {
    return estadoDurmiendo_;
}

IStateR16* ControllerRobot16::getEstadoConCarga() {
    return estadoConCarga_;
}

IStateR16* ControllerRobot16::getEstadoTrabajando() {
    return estadoTrabajando_;
}

void ControllerRobot16::setEstado(IStateR16* estado) {
    estadoActual_ = estado;
}

ControllerRobot16::~ControllerRobot16() {
    delete shMem_R14_R16_Data_;
    
    delete estadoDurmiendo_;
    delete estadoTrabajando_;
    delete estadoConCarga_;
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

        // Se intenta despertar al Robot 11 si es que este está dormido
        if (shMem_R14_R16_Data_->estaRobot14Bloqueado()) {
            Logger::logMessage(Logger::TRACE, "Se despierta al Robot14 de la Cinta15");

            shMem_R14_R16_Data_->setEstadoBloqueoRobot16(false);
            semR14_.signal();
        }
    }
    else {
        Logger::logMessage(Logger::TRACE, "Cinta15 no pudo ser movida.");        
    }

    liberar_shMem_R14_R16();
    return cintaMovida;
}

void ControllerRobot16::depositarCajaEnAPT(Caja caja) {
    // TODO:
    Logger::logMessage(Logger::IMPORTANT, "Se tira Caja hasta implementación "
    "del Despacho y APT");
}

