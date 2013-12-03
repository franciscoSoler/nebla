
#include "AbstractMessageQueue.h"

namespace IPC {
    
AbstractMessageQueue::AbstractMessageQueue(std::string IPCName, long idEmisor,
                        TipoAgente idDuenioEstaCola,
                        TipoAgente idDuenioColaRemota) : IPCObject(IPCName)
                                ,idEmisor(idEmisor)
                                ,idDuenioEstaCola_(idDuenioEstaCola)
                                ,idDuenioColaRemota_(idDuenioColaRemota)
                                ,id(0){
}

AbstractMessageQueue::~AbstractMessageQueue() {
}

int AbstractMessageQueue::createMessageQueue(const char *fileName, int id) {
	return this->getId(fileName, id, 0666|IPC_CREAT|IPC_EXCL);
}

int AbstractMessageQueue::getMessageQueue(const char *fileName, int id) {
    // Caso base de la recursividad: Código REEE entendible
    this->idIPC = id;
    strcpy(this->dirIPC, fileName);
    return this->getId(fileName, id, 0666);
}

void AbstractMessageQueue::destroy () {
	msgctl ( this->id,IPC_RMID,NULL );
}

int AbstractMessageQueue::getId(const char *fileName, int id, int flags) {
			
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		sprintf(this->buffer, "%s - Fallo la operacion ftok: %s", getIPCName().c_str(), strerror(errno));
                throw Exception(std::string(this->buffer));
	}
	
	this->id = msgget ( clave ,flags );
	if ( this->id == -1 ) {
		sprintf(this->buffer, "%s - Fallo la operacion msgget: %s", getIPCName().c_str(), strerror(errno));
                throw Exception(std::string(this->buffer));
	}
	return 0;
}

int AbstractMessageQueue::enviar(const void *dato, size_t size) {
    int resultado = msgsnd(this->id, dato, size, 0);
    if (resultado == -1) {
        sprintf(this->buffer, "%s - Fallo la operacion msgsnd: %s", getIPCName().c_str(), strerror(errno));
        throw Exception(std::string(this->buffer));
    }
    return resultado;
}

int AbstractMessageQueue::recibir(int tipo, void *buffer, size_t size) {
    int resultado = msgrcv(this->id, buffer, size, tipo, 0);
    if (resultado == -1) {
        sprintf(this->buffer, "%s - Fallo la operacion msgrcv: %s", getIPCName().c_str(), strerror(errno));
        throw Exception(std::string(this->buffer));
    }
    return resultado;
}

}