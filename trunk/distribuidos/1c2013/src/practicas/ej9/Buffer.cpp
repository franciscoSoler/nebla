#include "Buffer.h"



Buffer::Buffer (int cantidadProductores) {
	// Obtengo semaforo de control de acceso a la memoria compartida
	semControl = Semaphore();	
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);

	// Obtengo semaforos de control de los procesos
	semProductores = Semaphore();
	semProductores.getSemaphore((char *)DIRECTORY, ID_SEMPRODUCTORES, cantidadProductores);
	
	// Obtengo la memoria compartida
	shMem = CountingSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	
	this->cantidadProductores = cantidadProductores;
}

Buffer::~Buffer() { 
}

bool Buffer::producir(int nroProductor) {
	
	int resultado = 0;
	resultado = semProductores.wait(nroProductor);
	{	
		if (resultado < 0) return false;
		resultado = semControl.wait(0);
		{
			if (resultado < 0) return false;
			
			int *p =  shMem.readInfo();
			*p = *p + 1;
			
			if ((*p) == cantidadProductores) {
				for (int i = 0; i<cantidadProductores ;i++) semProductores.signal(i);		
				*p=0;
				shMem.writeInfo(p);
			}
		}
		resultado = semControl.signal(0);
		if (resultado < 0) return false;
	}
	return true;
}
