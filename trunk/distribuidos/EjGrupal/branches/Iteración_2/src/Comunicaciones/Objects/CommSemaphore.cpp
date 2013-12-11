#include "CommSemaphore.h"
#include <Logger/Logger.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <middlewareCommon.h>
#include <Comunicaciones/Objects/CommPacketWrapper.h>
#include <sstream>

namespace COMM {  
    
CommSemaphore::CommSemaphore(std::string CommName, long idEmisor, TipoAgente typeDuenioSem, 
        TipoAgente typeDuenioSemRemoto) : CommObject(CommName),
                                        idEmisor_(idEmisor),
                                        typeDuenioSem_(typeDuenioSem),
                                        typeDuenioSemRemoto_(typeDuenioSemRemoto)
                                                
{
    senderMsgQueue_ = IPC::MsgQueue("CommSemaphore - senderMsgQueue");
    receiverMsgQueue_ = IPC::MsgQueue("CommSemaphore - receiverMsgQueue");
}

CommSemaphore::~CommSemaphore() 
{
}

void CommSemaphore::initializeSemaphore(int numSem, int val)
{
    // este deberia enviar la cantidad de mensajes como tiene que ser!!!!
    if (val != 0) {
        this->signal(numSem);
    }
} 

void CommSemaphore::getSemaphore(const char *fileName, int id, int qty) {
    this->initializeQueues();
    std::string key = this->createKey("sem", fileName, id);
    this->commId_ = this->findCommId( key );
}

void CommSemaphore::initializeQueues() {
    try {
        char buffer[TAM_BUFFER];
        sprintf(buffer, "%s inicializoQueueSalida: dirIPC %s, tipoAgente: %d", this->commName_.c_str(), DIRECTORY_COMM, this->typeDuenioSem_);
        Logger::logMessage(Logger::COMM, buffer);
        sprintf(buffer, "%s inicializoQueueEntrada: dirIPC %s, tipoAgente: %d", this->commName_.c_str(), DIRECTORY_SEM, ID_COMM_SEM_ENTRADA);
        Logger::logMessage(Logger::COMM, buffer);
        
        
        this->senderMsgQueue_.getMsgQueue(DIRECTORY_COMM, this->typeDuenioSem_);
        this->receiverMsgQueue_.getMsgQueue(DIRECTORY_SEM, this->typeDuenioSem_);
    }
    catch(Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

//-----------------------------------------------------------------------------
void CommSemaphore::wait(int numSem)
{	    
    MsgAgenteReceptor msg;
    
    try {
        this->receiverMsgQueue_.recv(this->idEmisor_, msg);
    }
    catch(Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void CommSemaphore::signal(int idReceptor)
{
    CommPacketWrapper wrapper;
    wrapper.setDirIPC( DIRECTORY_SEM );
    wrapper.setIdDirIPC( ID_COMM_SEM_ENTRADA );
    wrapper.setSenderId( this->idEmisor_ );
    wrapper.setReceiverType( typeDuenioSemRemoto_ );
    wrapper.setReceiverId( idReceptor + 1 );
    MsgCanalSalidaAgente msg;
    wrapper.createPacketForSemaphores(msg);
    
    try {
        this->senderMsgQueue_.send(msg);
    }
    catch(Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
} 	

}
