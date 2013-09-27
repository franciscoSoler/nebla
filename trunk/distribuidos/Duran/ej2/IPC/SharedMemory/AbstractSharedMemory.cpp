#include "AbstractSharedMemory.h"

AbstractSharedMemory::AbstractSharedMemory () {
}

AbstractSharedMemory::~AbstractSharedMemory() { 
}

int AbstractSharedMemory::getSharedMemory(char *fileName, int id) {
	int result = this->getId(fileName, id, 0666);
	if (result < 0) return -1;
	return this->attachMemory();
}

int AbstractSharedMemory::createSharedMemory(char *fileName, int id) {
	int result = this->getId(fileName, id, 0666|IPC_CREAT|IPC_EXCL);
	if (result < 0) return -1;
	return this->attachMemory();
}
	
int AbstractSharedMemory::destroy(void) {
	// Me desattacho de la memoria
	shmdt ( (void *)this->data );
	// Si no quedan procesos attachados, libero la memoria
	if (! this->getCantAttachProcesses()) {
		shmctl(this->id, IPC_RMID, NULL); 
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
	char buffer[255];
	void *shmaddr = shmat(this->id, NULL, 0);
	if (shmaddr == (void *)-1)
	{
		strcpy(buffer, "Error en Shared Memory - shmat:");
		strcat(buffer, strerror(errno));
		strcat(buffer, "\n");	
		write(fileno(stdout), buffer, strlen(buffer)); 
		return -1;
	}
	data = shmaddr;
	return 0;
}

int AbstractSharedMemory::getId(char *fileName, int id, int flags) {
	char buffer[255];
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		strcpy(buffer, "Error en SharedMemory - ftok:");
		strcat(buffer, strerror(errno));	
		strcat(buffer, "\n");
		write(fileno(stdout), buffer, strlen(buffer)); 
		return -1;
	}

	this->id =  shmget( clave, this->getMemorySize() , flags);
	if ( this->id == -1 ) {
		strcpy(buffer, "Error en SharedMemory - shmget:");
		strcat(buffer, strerror(errno));
		strcat(buffer, "\n");
		write(fileno(stdout), buffer, strlen(buffer)); 
		return -1;
	}
	return 0;
}
