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

	Semaphore semLec = Semaphore();
	semLec.getSemaphore((char *) DIRECTORY, ID_SEMLEC, 1);
	semLec.destroy();

	Semaphore semEsc = Semaphore();
	semEsc.getSemaphore((char *) DIRECTORY, ID_SEMESC, 1);
	semEsc.destroy();

	// Creando memoria compartida
	ControlDataSharedMemory shMem = ControlDataSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	
	shMem.destroy();

	return 0;
}

