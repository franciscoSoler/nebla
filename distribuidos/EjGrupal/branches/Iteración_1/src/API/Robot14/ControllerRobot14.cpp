#include "ControllerRobot14.h"
#include "MsgQueue.h"
#include <Logger/Logger.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <API/Objects/DataSM_R11_R14.h>
#include <API/Objects/Util.h>
#include <Exceptions/Exception.h>
#include <sstream>

ControllerRobot14::ControllerRobot14() {
    try {
        Logger::getInstance();
        // Util::getInstance();
        Logger::setProcessInformation("Robot14:");
        estaMutex_R11_R14_tomado_ = false;
        
        shMem_R11_R14_Data_ = new DataSM_R11_R14();
        shMem_R14_R16_Data_ = new DataSM_R14_R16();
             
        // Se crean los IPCs
        shMem_R11_R14_ = IPC::SharedMemory<DataSM_R11_R14>("ShMem_R11_R14");
        shMem_R11_R14_.getSharedMemory(DIRECTORY_ROBOT_11, SM_R11_R14_ID);
        
        shMem_R14_R16_ = IPC::SharedMemory<DataSM_R14_R16>("ShMem_R14_R16");
        shMem_R14_R16_.getSharedMemory(DIRECTORY_ROBOT_14, SM_R14_R16_ID);
   
        semMutex_shMem_R11_R14_ = IPC::Semaphore ("semMutex_shMem_R11_R14");
        semMutex_shMem_R11_R14_.getSemaphore(DIRECTORY_ROBOT_11, SEM_MUTEX_SM_R11_R14_ID, 1);
        
        semMutex_shMem_R14_R16_ = IPC::Semaphore ("semMutex_shMem_R14_R16");
        semMutex_shMem_R14_R16_.getSemaphore(DIRECTORY_ROBOT_14, SEM_MUTEX_SM_R14_R16_ID, 1);
    
        semR11_Cinta13_ = IPC::Semaphore ("semR11_Cinta13");
        semR11_Cinta13_.getSemaphore(DIRECTORY_ROBOT_11, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
        
        semR14_Cinta13 = IPC::Semaphore("semR14_Cinta13");
        semR14_Cinta13.getSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA13_ID, 1);

        semR14_Cinta15 = IPC::Semaphore("semR14_Cinta15");
        semR14_Cinta15.getSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA15_ID, 1);
        
        inputQueueR16_Cinta15_ = IPC::MsgQueue("inputQueueR16_Cinta15");
        inputQueueR16_Cinta15_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CINTA15_INPUT_ID);

        // Creo los estados
        estadoDurmiendo_ = new StateR14_Durmiendo(this);
        estadoTrabajando_ = new StateR14_Trabajando(this);
        estadoConCarga_ = new StateR14_ConCarga(this);
        estadoActual_ = estadoDurmiendo_;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }    
}

void ControllerRobot14::comenzarATrabajar() {
    try {
        Logger::setProcessInformation("Robot14 - comenzarATrabajar:");
        // Se bloquea esperando que lleguen cajas
        obtener_shMem_R11_R14();

        if ( shMem_R11_R14_Data_->estaRobot14Bloqueado() ) {
            Logger::logMessage(Logger::TRACE, "Se bloquea esperando que un Robot11 coloque cajas");
            liberar_shMem_R11_R14();
            semR14_Cinta13.wait();

            obtener_shMem_R11_R14();
            shMem_R11_R14_Data_->setEstadoBloqueoRobot14(true);
            Logger::logMessage(Logger::TRACE, "Es desbloqueado. Procede a trabajar");
            liberar_shMem_R11_R14();
        }
        else {
            Logger::logMessage(Logger::TRACE, "Hay cajas en las cintas. Procede a trabajar");
            liberar_shMem_R11_R14();
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot14::estaRobot11BloqueadoEnCinta(uint nroCinta) {
    try {
        Logger::setProcessInformation("Robot14 - estaRobot11BloqueadoEnCinta:");

        obtener_shMem_R11_R14();
        bool robotBloqueado = shMem_R11_R14_Data_->estaRobot11Bloqueado(nroCinta);
        liberar_shMem_R11_R14();

        return robotBloqueado;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

uint ControllerRobot14::obtenerCantidadDeCajasEnCinta(uint nroCinta) {
    try {
        Logger::setProcessInformation("Robot14 - obtenerCantidadDeCajasEnCinta:");

        obtener_shMem_R11_R14();
        uint elementosEnCinta = shMem_R11_R14_Data_->getCantidadElementosEnCinta(nroCinta);
        liberar_shMem_R11_R14();

        return elementosEnCinta;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot14::moverCinta(uint nroCinta) {
    try {
        Logger::setProcessInformation("Robot14 - moverCinta:");
        return estadoActual_->moverCinta(nroCinta);
    } 
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot14::tomarCaja(Caja & unaCaja, uint nroCinta) {
    try {
        Logger::setProcessInformation("Robot14 - tomarCaja:");
        return estadoActual_->tomarCaja(unaCaja, nroCinta);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControllerRobot14::depositarCaja(Caja unaCaja) {
    try {
        Logger::setProcessInformation("Robot14 - depositarCaja:");
        estadoActual_->depositarCaja(unaCaja);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot14::moverCintaEnEstadoDurmiendo(uint nroCinta) {
    // Elige la cinta de trabajo y pasa a estado Trabajando
    obtener_shMem_R11_R14();
    
    shMem_R11_R14_Data_->setCintaDeTrabajoRobot14(nroCinta); 
    setEstado( getEstadoTrabajando() );

    bool cintaMovida = avanzarCinta(nroCinta);

    liberar_shMem_R11_R14();
    return cintaMovida;
}

bool ControllerRobot14::moverCintaEnEstadoTrabajando(uint nroCinta) {
    obtener_shMem_R11_R14();
    bool cintaMovida = avanzarCinta(nroCinta);
    liberar_shMem_R11_R14();
    return cintaMovida;
}

bool ControllerRobot14::tomarCajaEnEstadoTrabajando(Caja & unaCaja, uint nroCinta) {
    obtener_shMem_R11_R14();
    bool retVal = shMem_R11_R14_Data_->retirarCajaDeCinta(nroCinta, unaCaja);
    
    if (retVal) {
        sprintf(buffer_, "Retira caja de la cinta N°%u", nroCinta);
        Logger::logMessage(Logger::TRACE, buffer_);
        
        setEstado(getEstadoConCarga());
        // El robot14 ahora no está trabajando sobre ninguna cinta, dado que 
        // está intentando depositar la misma en la Cinta15
        shMem_R11_R14_Data_->setCintaDeTrabajoRobot14(0);
    }
    else {
        sprintf(buffer_, "No se pudo retirar caja de la Cinta13 N°%u", nroCinta);
        Logger::logMessage(Logger::TRACE, buffer_);
    }
    
    liberar_shMem_R11_R14();
    return retVal;
}

void ControllerRobot14::depositarCajaEnEstadoConCarga(Caja & caja) {
    depositarCajaEnCinta15(caja);
    
    obtener_shMem_R11_R14();
    if (shMem_R11_R14_Data_->getCantidadElementosEnCinta(1) == 0 && 
        shMem_R11_R14_Data_->getCantidadElementosEnCinta(2) == 0) {

        setEstado( getEstadoDurmiendo() );
        shMem_R11_R14_Data_->setEstadoBloqueoRobot14(true);
        Logger::logMessage(Logger::TRACE, "No hay cajas en ambas cintas. Setea su estado a bloqueado");
    }
    else {
        setEstado( getEstadoTrabajando() );
        Logger::logMessage(Logger::TRACE, "Hay cajas. Sigue trabajando");
    }
    
    liberar_shMem_R11_R14();
}

IStateR14* ControllerRobot14::getEstadoDurmiendo() {
    return estadoDurmiendo_;
}

IStateR14* ControllerRobot14::getEstadoConCarga() {
    return estadoConCarga_;
}

IStateR14* ControllerRobot14::getEstadoTrabajando() {
    return estadoTrabajando_;
}

void ControllerRobot14::setEstado(IStateR14* estado) {
    estadoActual_ = estado;
}

ControllerRobot14::~ControllerRobot14() {
    delete shMem_R11_R14_Data_;
    delete shMem_R14_R16_Data_;

    delete estadoDurmiendo_;
    delete estadoConCarga_;
    delete estadoTrabajando_;
}

bool ControllerRobot14::obtener_shMem_R11_R14() {
    if (! estaMutex_R11_R14_tomado_) {
        semMutex_shMem_R11_R14_.wait();
        // Logger::logMessage(Logger::TRACE, "Accede a memoria compartida R11-R14");
        shMem_R11_R14_.read(shMem_R11_R14_Data_);

        estaMutex_R11_R14_tomado_ = true;
        return true;
    }
    else {
        return false;
    }

}

bool ControllerRobot14::liberar_shMem_R11_R14() {
    if ( estaMutex_R11_R14_tomado_ ) {
        shMem_R11_R14_.write(shMem_R11_R14_Data_);
        // Logger::logMessage(Logger::TRACE, "Libera memoria compartida R11-R14");
        semMutex_shMem_R11_R14_.signal();

        estaMutex_R11_R14_tomado_ = false;
        return true;
    }
    else {
        return false;
    }
}

void ControllerRobot14::obtener_shMem_R14_R16() {
    semMutex_shMem_R14_R16_.wait();
    // Logger::logMessage(Logger::TRACE, "Accede a memoria compartida R14-R16");
    shMem_R14_R16_.read(shMem_R14_R16_Data_);
}

void ControllerRobot14::liberar_shMem_R14_R16() {
    shMem_R14_R16_.write(shMem_R14_R16_Data_);
    // Logger::logMessage(Logger::TRACE, "Libera memoria compartida R14-R16");
    semMutex_shMem_R14_R16_.signal();
}

bool ControllerRobot14::avanzarCinta(uint nroCinta) {
    bool cintaMovida = shMem_R11_R14_Data_->moverCinta(nroCinta);
    if (cintaMovida) {

        sprintf(buffer_, "Cinta13 N°%u es movida. %s", nroCinta, 
                shMem_R11_R14_Data_->cintaToString(nroCinta).c_str()); 
        Logger::logMessage(Logger::DEBUG, buffer_);

        // Se intenta despertar al Robot 11 si es que este está dormido
        if (shMem_R11_R14_Data_->estaRobot11Bloqueado(nroCinta)) {

            sprintf(buffer_, "Se despierta al Robot11 de la Cinta13 N°%u", nroCinta);
            Logger::logMessage(Logger::TRACE, buffer_);

            shMem_R11_R14_Data_->setEstadoBloqueoRobot11(nroCinta, false);
            semR11_Cinta13_.signal(nroCinta-1);
        }
    }
    else {
        sprintf(buffer_, "Cinta13 N°%u no pudo ser movida.", nroCinta);
        Logger::logMessage(Logger::TRACE, buffer_);        
    }
    
    return cintaMovida;
}

void ControllerRobot14::depositarCajaEnCinta15(Caja unaCaja) {
    try {
        bool cajaDepositada = false;
        
        while (! cajaDepositada) {
            obtener_shMem_R14_R16();

            // Se intenta depositar la caja. Sino se puede, se intenta mover 
            // la cinta. En caso negativo, el Robot se bloquea        
            // TODO: Candidato para meter un ChainOfResponsability
            if ( estaCinta15Llena() ) {
                Logger::logMessage(Logger::TRACE, "Cinta15 llena. Robot se bloquea.");
                bloquearRobot();
                continue;
            }
            else if (shMem_R14_R16_Data_->insertarCajaEnCinta(unaCaja)) {
                sprintf(buffer_, "Caja depositada. Cinta15: %s", 
                        shMem_R14_R16_Data_->cintaToString().c_str());
                Logger::logMessage(Logger::DEBUG, buffer_);
                cajaDepositada = true;
                
                Logger::logMessage(Logger::TRACE, "Envío de mensaje a Robot16");
                inputQueueR16_Cinta15_.send( mensaje_ );
            }
            else if (estaRobot16Trabajando()) {
                Logger::logMessage(Logger::TRACE, "Robot16 trabajando en la "
                "Cinta15. Robot se bloquea");
                bloquearRobot();
                continue;
            }
            else if (shMem_R14_R16_Data_->moverCinta()) {
                Logger::logMessage(Logger::TRACE, "Primer lugar ocupado. Mueve cinta");
                
                if (! shMem_R14_R16_Data_->insertarCajaEnCinta(unaCaja) ) {
                    throw Exception("Error en Cinta15. No se pudo colocar una "
                        "caja después de haber avanzado la cinta");
                }
                
                sprintf(buffer_, "Caja depositada. Cinta15: %s", 
                        shMem_R14_R16_Data_->cintaToString().c_str());
                Logger::logMessage(Logger::DEBUG, buffer_);
                cajaDepositada = true;
                
                Logger::logMessage(Logger::TRACE, "Envío de mensaje a Robot16");
                inputQueueR16_Cinta15_.send( mensaje_ );
            }
            else {
                Logger::logMessage(Logger::TRACE, "No se pudo mover cinta. Robot se bloquea");
                bloquearRobot();
                continue;
            }

            liberar_shMem_R14_R16();
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControllerRobot14::estaRobot16Trabajando() const {
    return shMem_R14_R16_Data_->estaRobot16Trabajando();
}

bool ControllerRobot14::estaCinta15Llena() const { 
    return shMem_R14_R16_Data_->getCantidadElementosEnCinta() == CINTA_15_CAPACITY;
}

void ControllerRobot14::bloquearRobot() {
    shMem_R14_R16_Data_->setEstadoBloqueoRobot14(true);
    liberar_shMem_R14_R16();        
    
    semR14_Cinta15.wait();
    Logger::logMessage(Logger::TRACE, "Es liberado.");
}
