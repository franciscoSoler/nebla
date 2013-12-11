#ifndef COMM_SEM_MUTEX_H_
#define	COMM_SEM_MUTEX_H_

#include "CommSemaphore.h"
#include "CommPacketWrapper.h"
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include "Logger.h"
#include <sstream>

namespace COMM {

template< class T >
class CommSemaphoreMutex : public CommSemaphore {
public:
    CommSemaphoreMutex(std::string CommName = "", long idEmisor = 0,
                        TipoAgente typeDuenioSem = ID_TIPO_VACIO) : 
                                CommSemaphore (CommName, idEmisor, typeDuenioSem),
                                cantShMen(0) {
        senderMsgQueue_ = IPC::MsgQueue("CommSemaphoreMutex - senderMsgQueue");
        receiverMsgQueue_ = IPC::MsgQueue("CommSemaphoreMutex - receiverMsgQueue");
    }
                            
    void getSemaphore(const char *fileName, int id, int qty) {
        initializeQueues();
        this->cantShMen = qty;

        //ahora tengo que conseguir desde el id las cosas de la shMem para luego hacer getshMem
        // char dirIPC[DIR_FIXED_SIZE];
        // int idIPC;
        //get desde un id a idIPC y dirIPC
    }
    
    void setShMem(const char * dirIPC, int idIPC, int numSem = 0) {        
        std::string key = this->createKey("shMem", dirIPC, idIPC);
        int commId = this->findCommId( key );

        /* Logger::logMessage(Logger::IMPORTANT, "Key: " + key);
        ss.str("");
        ss << commId_;
        Logger::logMessage(Logger::IMPORTANT, "Value: " + ss.str()); */
        
        this->idShMem[numSem] = commId;
        this->shMem[numSem].getSharedMemory(dirIPC, idIPC);
    }
    
    void wait(int numSem = 0) {
        CommPacketWrapper wrapper;
        wrapper.setIdShMem( this->idShMem[numSem] );
        wrapper.setReceiverId( this->idShMem[numSem] );
        wrapper.setReceiverType( ID_TIPO_PEDIDO_MEMORIA );
        wrapper.setSenderId( this->idEmisor_ );
        wrapper.setSenderType( this->typeDuenioSem_ );

        MsgCanalSalidaAgente msg;
        wrapper.createPacketRequestShMem(msg);

        try {
            char bufferr[TAM_BUFFER];
            sprintf(bufferr, "%s wait: envio el pedido de la shMem, idEmisor %ld, tipoEmisor: %d", 
                    this->commName_.c_str(), msg.mtype, this->typeDuenioSem_);
            Logger::logMessage(Logger::COMM, bufferr);
            
            this->senderMsgQueue_.send(msg);
            
           
            
            // ahora recibo la shMem y la guardo en la shMem
            MsgAgenteReceptor msg;
            this->receiverMsgQueue_.recv(this->idEmisor_, msg);

            Logger::logMessage(Logger::COMM, "recibi la shMem");
            
            // guardo lo recibido en la shMem
            T buffer;
            memcpy(&buffer, msg.msg, sizeof(T));
            this->shMem[numSem].write(&buffer);
        }
        catch(Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            abort();
        }
    }

    void signal(int numSem = 0) {
        // enviarle al broker la shMem diciendo que termine de utilizarla
        T buffer;
        this->shMem[numSem].read(&buffer);
        
        CommPacketWrapper wrapper;
        wrapper.setIdShMem( this->idShMem[numSem] );
        wrapper.setReceiverId( this->idShMem[numSem] );
        wrapper.setReceiverType( ID_TIPO_MEMORIA );
        wrapper.setSenderId( this->idEmisor_ );

        MsgCanalSalidaAgente msg;
        wrapper.createPacketReturnShMem(msg, buffer);

        try {
            this->senderMsgQueue_.send(msg);
            CommSemaphore::wait(numSem);
        }
        catch(Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            abort();
        }
    }

private:
    int cantShMen;
    int idShMem[SEM_ARRAY_MAX_SIZE];
    IPC::SharedMemory<T> shMem[SEM_ARRAY_MAX_SIZE];
};
}

#endif	/* SEMAPHOREMUTEX_H */

