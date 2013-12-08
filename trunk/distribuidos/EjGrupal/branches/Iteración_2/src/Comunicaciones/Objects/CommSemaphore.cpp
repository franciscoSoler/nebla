#include "CommSemaphore.h"
#include <Logger/Logger.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <middlewareCommon.h>

namespace COMM {

CommSemaphore::CommSemaphore(std::string CommName, TipoAgente idDuenioSem, 
        TipoAgente idDuenioSemRemoto) : CommObject(CommName),
                                        idDuenioSem_(idDuenioSem),
                                        idDuenioSemRemoto_(idDuenioSemRemoto)
                                                
{
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
    initializeQueues(fileName, id);
}

void CommSemaphore::initializeQueues(const char *fileName, int id) {
    // Get the ID of the CommObject from the ConfigFile
    std::stringstream ss;
    ss << id;

    std::string key;
    key += "sem-";
    key += fileName;
    key += "-";
    key += ss.str();

    findCommId();
    
    try {
        this->senderMsgQueue_.getMsgQueue(DIRECTORY_COMM, this->idDuenioSem_);
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
        this->receiverMsgQueue_.recv(this->commId_ + numSem, &msg);
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
    wrapper.setSenderId( ID_COMM_SEM_SALIDA );
    wrapper.setReceiverType( idDuenioSemRemoto_ );
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
