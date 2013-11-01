#include "AbstractSharedMemory.h"

#include "../IPCException.h"

AbstractSharedMemory::AbstractSharedMemory () {
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
	shmdt ( (void *)this->data );
	// Si no quedan procesos attachados, libero la memoria
	//if (! this->getCantAttachProcesses()) {
		shmctl(this->id, IPC_RMID, NULL); 
	//}
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
		char error[255];
		sprintf(error, "Fallo la operacion shmat: %s", strerror(errno));
		char className[255];
		strcpy(className, "AbstractSharedMemory");
		throw IPCException((const char*)className, error);
	}
	data = shmaddr;
	return 0;
}

int AbstractSharedMemory::getId(const char *fileName, int id, int flags) {
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		char error[255];
		sprintf(error, "Fallo la operacion ftok: %s", strerror(errno));
		char className[255];
		strcpy(className, "AbstractSharedMemory");
		throw IPCException((const char*)className, error);
	}

	this->id = shmget( clave, this->getMemorySize() , flags);
	if ( this->id == -1 ) {
		char error[255];
		sprintf(error, "Fallo la operacion shmget: %s", strerror(errno));
		char className[255];
		strcpy(className, "AbstractSharedMemory");	
		throw IPCException((const char*)className, error);
	}
	return 0;
}
