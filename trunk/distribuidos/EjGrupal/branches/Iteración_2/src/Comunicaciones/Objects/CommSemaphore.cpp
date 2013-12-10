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
    senderMsgQueue_ = IPC::MsgQueue("senderMsgQueue");
    receiverMsgQueue_ = IPC::MsgQueue("receiverMsgQueue");
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
    this->initializeQueues(fileName, id);
    std::string key = this->createKey("sem", fileName, id);
    this->commId_ = this->findCommId( key );
}

void CommSemaphore::initializeQueues(const char *fileName, int id) {
    try {
        this->senderMsgQueue_.getMsgQueue(DIRECTORY_COMM, this->typeDuenioSem_);
        this->receiverMsgQueue_.getMsgQueue(DIRECTORY_SEM, ID_COMM_SEM_ENTRADA);
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
        this->receiverMsgQueue_.recv(this->commId_ + numSem, msg);
    }
    catch(Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void CommSemaphore::signal(int numSem)
{
    CommPacketWrapper wrapper;
    wrapper.setDirIPC( DIRECTORY_SEM );
    wrapper.setIdDirIPC( ID_COMM_SEM_ENTRADA );
    wrapper.setSenderId( this->idEmisor_ );
    wrapper.setReceiverType( typeDuenioSemRemoto_ );
    wrapper.setReceiverId( this->commId_ + numSem );
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
