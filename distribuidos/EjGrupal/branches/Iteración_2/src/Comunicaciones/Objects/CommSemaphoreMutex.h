#ifndef SEMAPHOREMUTEX_H
#define	SEMAPHOREMUTEX_H

#include "CommSemaphore.h"
#include "CommPacketWrapper.h"
#include "Logger.h"

namespace COMM {

template< class T, unsigned int SIZE = sizeof(T) >
class CommSemaphoreMutex : public CommSemaphore {
public:
    CommSemaphoreMutex(std::string CommName = "", 
                        TipoAgente idDuenioSem = ID_TIPO_VACIO) : 
                                CommSemaphore (CommName, idDuenioSem) {}
                            
    void getSemaphore(const char *fileName, int id, int qty) {
        initializeQueues(fileName, id);

        //ahora tengo que conseguir desde el id las cosas de la shMem para luego hacer getshMem
        char dirIPC[DIR_FIXED_SIZE];
        int idIPC;
        //get desde un id a idIPC y dirIPC
    }
    
    void setShMem(const char * dirIPC, int idIPC) {
        this->shMem.getSharedMemory(dirIPC, idIPC);
    }
    
    void wait(int numSem = 0) {
        CommPacketWrapper wrapper;
        wrapper.setIdShMem( this->commId_ + numSem );
        wrapper.setReceiverId( this->commId_ + numSem );
        wrapper.setReceiverType( ID_TIPO_PEDIDO_MEMORIA );
        wrapper.setSenderId( ID_COMM_SEM_SALIDA );

        MsgCanalSalidaAgente msg;
        wrapper.createPacketRequestShMem(msg);

        try {
            this->senderMsgQueue_.send(msg);

            // ahora recibo la shMem y la guardo en la shMem
            MsgAgenteReceptor msg;
            this->receiverMsgQueue_.recv(this->commId_, &msg);

            // guardo lo recibido en la shMem
            T buffer;
            memcpy(&buffer, msg.msg, sizeof(T));
            this->shMem.write(&buffer);
        }
        catch(Exception & e) {
            Logger::logMessage(Logger::ERROR, e.get_error_description());
            abort();
        }
    }

    void signal(int numSem = 0) {
        // enviarle al broker la shMem diciendo que termine de utilizarla
        T buffer;
        this->shMem.read(&buffer);
        
        CommPacketWrapper wrapper;
        wrapper.setIdShMem( this->commId_ + numSem );
        wrapper.setReceiverId( this->commId_ + numSem );
        wrapper.setReceiverType( ID_TIPO_MEMORIA );
        wrapper.setSenderId( ID_COMM_SEM_SALIDA );

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
    IPC::SharedMemory<T> shMem;
};
}

#endif	/* SEMAPHOREMUTEX_H */

