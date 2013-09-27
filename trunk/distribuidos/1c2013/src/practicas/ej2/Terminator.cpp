#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./IPC/SharedMemory/ControlDataSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"


int main(int argc, char **argv)
{	
	Semaphore semControl = Semaphore();
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	semControl.destroy();

	
	Semaphore semDoor = Semaphore();
	semDoor.getSemaphore((char *) DIRECTORY, ID_SEMDOOR, CANT_DOORS);	
	semDoor.destroy();

	Semaphore semFull = Semaphore();
	semFull.getSemaphore((char *) DIRECTORY, ID_SEMFULL, 1);
	semFull.destroy();

	// Creando memoria compartida
	ControlDataSharedMemory shMem = ControlDataSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	shMem.destroy();

	return 0;
}

