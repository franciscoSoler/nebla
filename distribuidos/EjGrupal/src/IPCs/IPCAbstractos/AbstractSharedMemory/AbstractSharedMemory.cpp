#include "AbstractSharedMemory.h"
#include <iostream>

namespace IPC {

AbstractSharedMemory::AbstractSharedMemory (std::string IPCName) : IPCObject(IPCName) {
}

AbstractSharedMemory::~AbstractSharedMemory() { 
}

int AbstractSharedMemory::getSharedMemory(const char *fileName, int id) {
	this->getId(fileName, id, 0666);
	return this->attachMemory();
}

int AbstractSharedMemory::createSharedMemory(const char *fileName, int id) {
	this->getId(fileName, id, 0666|IPC_CREAT|IPC_EXCL);
	return this->attachMemory();
}
	
int AbstractSharedMemory::destroy(void) {
    // Me desattacho de la memoria
    while (this->getCantAttachProcesses()) {
        if (shmdt ( (void *)this->data ) != 0)
            break;
    }
    
    // Si no quedan procesos attachados, libero la memoria
    if (shmctl(this->id, IPC_RMID, NULL) != 0) {
        sprintf(buffer, "%s Error - destroy: %s", getIPCName().c_str(), strerror(errno));
        throw Exception(std::string(buffer));
    }        
    return 0;
}

int AbstractSharedMemory::getCantAttachProcesses(void) const {
	shmid_ds estado;
	shmctl(this->id, IPC_STAT, &estado);
	return estado.shm_nattch;
}
	
int AbstractSharedMemory::attachMemory() {
	// Me attacho a la memoria dejando al SO que elija donde ubicar la memoria 
	//(atributo en NULL)y para lectura/escritura (atributo en 0)
	void *shmaddr = shmat(this->id, NULL, 0);
	if (shmaddr == (void *)-1) {
		sprintf(this->buffer, "%s - Fallo la operacion shmat: %s", getIPCName().c_str(), strerror(errno));
		throw Exception(std::string(this->buffer));
	}
	data = shmaddr;
	return 0;
}

int AbstractSharedMemory::getId(const char *fileName, int id, int flags) {
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		sprintf(this->buffer, "%s - Fallo la operacion ftok: %s", getIPCName().c_str(), strerror(errno));
                throw Exception(std::string(this->buffer));
	}

	this->id = shmget( clave, this->getMemorySize() , flags);
	if ( this->id == -1 ) {
		sprintf(this->buffer, "%s - Fallo la operacion shmget: %s", getIPCName().c_str(), strerror(errno));
		throw Exception(std::string(this->buffer));
	}
	return 0;
}

}