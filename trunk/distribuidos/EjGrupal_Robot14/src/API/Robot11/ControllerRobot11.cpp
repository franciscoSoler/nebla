#include "ControllerRobot11.h"
#include "Exception.h"
#include <CSO/Logger/Logger.h>
#include <Objects/Util.h>
#include <CSO/Exceptions/Exception.h>

ControllerRobot11::ControllerRobot11(uint nroCinta) {
    try {
        nroCinta_ = nroCinta;
        shMem_R11_R14_Data_ = new DataSM_R11_R14();
        Logger::getInstance();
        Util::getInstance();
        
        sprintf(buffer_, "Robot11 - CintaN°%u:", nroCinta);
        Logger::setProcessInformation(buffer_);
        
        // Se crean los IPCs
        shMem_R11_R14_ = IPC::SharedMemory<DataSM_R11_R14>("ShMem_R11_R14");
        shMem_R11_R14_.getSharedMemory(DIRECTORY, SM_R11_R14_ID);
   
        semMutex_shMem_R11_R14_ = IPC::Semaphore ("semMutex_shMem_R11_R14");
        semMutex_shMem_R11_R14_.getSemaphore(DIRECTORY, SEM_MUTEX_SM_R11_R14_ID, 1);
    
    
        semR11_Cinta13_ = IPC::Semaphore ("semR11_Cinta13");
        semR11_Cinta13_.getSemaphore(DIRECTORY, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
        
        semR14_ = IPC::Semaphore("semR14");
        semR14_.getSemaphore(DIRECTORY, SEM_R14_ID, 1);
       
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

Caja ControllerRobot11::tomarCaja() {
    //TODO: Acá iría parte del código de Fran. Yo por el momento no simulo a la 
    // cinta ni a la operación del Robot 11, sólo me aparecen Cajas automágicamente
    
    sleep( Util::generateRandomNumber(1, 5) );
    Caja unaCaja(1231);
    return unaCaja;
}

void ControllerRobot11::depositarCaja(Caja unaCaja) {
    try {
        bool cajaDepositada = false;
        while (! cajaDepositada) {
            obtener_shMem_R11_R14();

            // Se intenta depositar la caja. Sino se puede, se intenta mover 
            // la cinta. En caso negativo, el Robot se bloquea        
            // TODO: Candidato para meter un ChainOfResponsability
            if ( estaCintaLlena() ) {
                Logger::logMessage(Logger::TRACE, "Cinta13 llena. Robot se bloquea.");
                bloquearRobot();
                continue;
            }
            else if (shMem_R11_R14_Data_->insertarCajaEnCinta(nroCinta_, unaCaja)) {
                sprintf(buffer_, "Caja depositada. Cinta13 N°%u: %s", nroCinta_, 
                        shMem_R11_R14_Data_->cintaToString(nroCinta_).c_str());
                Logger::logMessage(Logger::DEBUG, buffer_);
                cajaDepositada = true;
            }
            else if (estaRobot14TrabajandoEnEstaCinta()) {
                Logger::logMessage(Logger::TRACE, "Robot14 trabajando en la "
                "cinta. Robot se bloquea");
                bloquearRobot();
                continue;
            }
            else if (shMem_R11_R14_Data_->moverCinta(nroCinta_)) {
                Logger::logMessage(Logger::TRACE, "Primer lugar ocupado. Mueve Cinta13");
                
                if (! shMem_R11_R14_Data_->insertarCajaEnCinta(nroCinta_, unaCaja) ) {
                    throw Exception("Error en Cinta13. No se pudo colocar una "
                        "caja después de haber avanzado la cinta");
                }
                
                sprintf(buffer_, "Caja depositada. Cinta13 N°%u: %s", nroCinta_, 
                        shMem_R11_R14_Data_->cintaToString(nroCinta_).c_str());
                Logger::logMessage(Logger::DEBUG, buffer_);
                cajaDepositada = true;
            }
            else {
                Logger::logMessage(Logger::TRACE, "No se pudo mover Cinta13. Robot se bloquea");
                bloquearRobot();
                continue;
            }
            
            // Si se logró colocar la caja, se despierta al Robot14 si este estaba
            // bloqueado
            if (shMem_R11_R14_Data_->estaRobot14Bloqueado()) {
                shMem_R11_R14_Data_->setEstadoBloqueoRobot14(false);
               Logger::logMessage(Logger::TRACE, "Robot14 despertado para trabajar en cinta");
               semR14_.signal();
            }

            liberar_shMem_R11_R14();
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}


ControllerRobot11::~ControllerRobot11() {
    delete shMem_R11_R14_Data_;
}

bool ControllerRobot11::estaRobot14TrabajandoEnEstaCinta() const {
    return (nroCinta_ == shMem_R11_R14_Data_->getCintaDeTrabajoRobot14());
}

bool ControllerRobot11::estaCintaLlena() const { 
    return shMem_R11_R14_Data_->getCantidadElementosEnCinta(nroCinta_) == CINTA_13_CAPACITY;
}

void ControllerRobot11::bloquearRobot() {
    shMem_R11_R14_Data_->setEstadoBloqueoRobot11(nroCinta_, true);
    liberar_shMem_R11_R14();        
    
    semR11_Cinta13_.wait(nroCinta_ - 1);
    Logger::logMessage(Logger::TRACE, "Es liberado.");
}

void ControllerRobot11::obtener_shMem_R11_R14() {
    semMutex_shMem_R11_R14_.wait();
    // Logger::logMessage(Logger::TRACE, "Accede a memoria compartida R11-R14");
    shMem_R11_R14_.read(shMem_R11_R14_Data_);
}

void ControllerRobot11::liberar_shMem_R11_R14() {
    shMem_R11_R14_.write(shMem_R11_R14_Data_);
    // Logger::logMessage(Logger::TRACE, "Libera memoria compartida R11-R14");
    semMutex_shMem_R11_R14_.signal();
}
