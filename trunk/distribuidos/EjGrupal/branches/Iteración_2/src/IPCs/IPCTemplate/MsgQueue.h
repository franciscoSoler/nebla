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
    char buffer_[200];

public:

    MsgQueue(std::string IPCName = "")
            :  IPCObject(IPCName),
               id(0) {}

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
        if (this->getId(fileName, id, 0666) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - getMsgQueue: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
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
    void send(const char buffer[], int msgSize) {
        if (msgsnd (this->id, (const void *) buffer, msgSize - sizeof(long), 0) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - send: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }
    }

    /*
     *
     */
    void recv( long type, char buffer[], int msgSize) {
        if (msgrcv (this->id, (void *) buffer, msgSize - sizeof(long), type, 0) == -1) {
            sprintf(buffer, "MsgQueue %s Error - rcv: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer);
        }
    }

    /*
     *
     */
    template<class T>
    void send( T& data ) {
        if (msgsnd (this->id, (const void *) & data, sizeof(T)-sizeof(long), 0) == -1) {
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
        if (msgrcv (this->id, (void *) & data, sizeof(T)-sizeof(long), type, 0) == -1) {
            sprintf(buffer_, "MsgQueue %s Error - rcv: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }
    }

};
}
#endif
