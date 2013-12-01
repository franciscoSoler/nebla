#ifndef MSGQUEUE_H_
#define MSGQUEUE_H_

#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <Exceptions/Exception.h>
#include <IPCs/IPCObject/IPCObject.h>
#include <middlewareCommon.h>
#include <Logger/Logger.h>


namespace IPC {

class MsgQueue : public IPCObject {    
private:
    int getId(const char *fileName, int id, int flags) {
        key_t clave = ftok (fileName, id);

        if ( clave == -1 ) {
            return -1;
        }

        this->id = msgget ( clave ,flags );
        if ( this->id == -1 ) {
            return -1;
        }

        return 0;
    }

    int id;
    long idEmisor;
    TipoAgente idTipoAgente;
    MsgQueue* colaMux;
    int idIPC;
    char dirIPC[DIR_FIXED_SIZE];
    char buffer_[200];
    
public:    
    
    MsgQueue(std::string IPCName = "", long idEmisor = 0, TipoAgente idTipoAgente = ID_TIPO_VACIO) 
            :  IPCObject(IPCName), 
               id(0),
               idEmisor(0),
               idTipoAgente(idTipoAgente),
               colaMux(NULL) {
        
    }
            
    virtual ~MsgQueue() {
        if ( colaMux != NULL) {
            delete colaMux;
        }
    }

    /* 
     * Metodo utilizado para crear la Cola de Mensajes, si ya 
     * existe una cola de mensajes con la misma clave el metodo falla.
     * El launcher es el encargado de crear los ipcs, y por lo tanto de 
     * invocar este metodo.
     */
    void create(const char *fileName, int id) {
        if (this->getId(fileName, id, 0666|IPC_CREAT|IPC_EXCL) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - create: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }
    }

    /* 
     * Metodo utilizado para obtener una Cola de Mensajes, si no existe
     * una cola de mensajes con la misma clave el metodo falla.
     */
    void getMsgQueue(const char *fileName, int id) {
        idIPC = id;
        strcpy(dirIPC, fileName);
        if (this->getId(fileName, id, 0666) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - getMsgQueue: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }
        
        // Caso base de la recursividad: Código REEE entendible
        if ( strcmp(fileName, DIRECTORY_MUX) ) {
            colaMux = new MsgQueue(DIRECTORY_MUX, idEmisor, idTipoAgente);
            colaMux->getMsgQueue(DIRECTORY_MUX, idTipoAgente);
        }
    }

    /* Pre: -
      * Post: -
      */
    void destroy(void) {
        if (msgctl ( this->id,IPC_RMID,NULL ) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - destroy: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }
    }
    
    /*
     *
     */
    template<class T>
    void send(long idReceptor, T& data ) {
        MsgAgenteReceptor msg;
        msg.mtype = MSG_MUX;
        msg.idEmisor = this->idEmisor;
        msg.idReceptor = idReceptor;
        msg.idIPCReceptor = this->idIPC;
        strcpy(msg.dirIPCReceptor, this->dirIPC);
        
        
        if ( sizeof(T) > MSG_QUEUE_FIXED_SIZE ) {
            sprintf(buffer_, "MsgQueue %s Error - send: Mensaje demasiado largo",
                    getIPCName().c_str());
            Logger::logMessage(Logger::ERROR, this->buffer_);
        }
        
        memcpy(msg.msg, &data, sizeof(T));
        this->send(msg);
    }

    /*
     * ESPANTOOOSOO!!!!
     */
    void send( MsgAgenteReceptor& data ) {
        if (msgsnd (this->colaMux->id, (const void *) & data, 
            sizeof(MsgAgenteReceptor) - sizeof(long), 0) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - send: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }
    }

    /*
     *
     */
    template<class T>
    void recv( long type, T& data) {
        MsgAgenteReceptor msg;
        this->recv(type, msg);
        
        memcpy(&data, msg.msg, sizeof(T));
    }
    
    void recv(long type, MsgAgenteReceptor& data ) {
        if (msgrcv (this->colaMux->id, (void *) & data, 
            sizeof(MsgAgenteReceptor) - sizeof(long), type, 0) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - send: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }
    }

};

}

#endif
