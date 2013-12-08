/* 
 * File:   SemaphoreMutex.cpp
 * Author: francisco
 * 
 * Created on 7 de diciembre de 2013, 13:00
 */

#include "CommSemaphoreMutex.h"
#include "CommPacketWrapper.h"
#include "Logger.h"

namespace COMM {

CommSemaphoreMutex::CommSemaphoreMutex(std::string CommName, int idShMem) : 
                                        CommSemaphore (CommName),
                                        idShMem_(idShMem)
{}

CommSemaphoreMutex::~CommSemaphoreMutex() {
}


void CommSemaphoreMutex::wait(int numSem)
{
    CommPacketWrapper wrapper;
    wrapper.setIdShMem(this->idShMem_);
    wrapper.setReceiverId( this->idShMem_ );
    wrapper.setReceiverType( ID_TIPO_PEDIDO_MEMORIA );
    wrapper.setSenderId( ID_COMM_SEM_SALIDA );
    
    MsgCanalSalidaAgente msg;
    wrapper.createPacketRequestShMem(msg);

    try {
        this->senderMsgQueue_.send(msg);
        CommSemaphore::wait(numSem);
    }
    catch(Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

}

