// Biblotecas estandar
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// Bibliotecas IPC
#include <sys/types.h>
#include <unistd.h>
// Clases propias
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/SharedMemory/ControlDataSharedMemory.h"
#include "common.h"

/* Programa cargado por los procesos Puerta. La implementación realizada
 * por el momento no contempla el tema del museo cerrado/abierto */

int main(int argc, char* argv[])
{
	// Creo los semáforos y la memoria compartida
	int cantPuertas = 0, nroPuerta = 0;
	// Buffer para mostrar eventos por salida estandar
	char buffer[255];
	// Estructura que se utiliza para leer y escribir desde la memoria compartida
	ControlData data;

	srand(getpid());	
	sscanf(argv[1], "%d", &cantPuertas); 

	Semaphore semControl = Semaphore();
	semControl.getSemaphore((char *) DIRECTORY, ID_SEMCONTROL, 1);
	Semaphore semDoor = Semaphore();
	semDoor.getSemaphore((char *) DIRECTORY, ID_SEMDOOR, CANT_DOORS);
	Semaphore semFull = Semaphore();
	semFull.getSemaphore((char *) DIRECTORY, ID_SEMFULL, 1);
	ControlDataSharedMemory shMem = ControlDataSharedMemory();
	shMem.getSharedMemory((char *) DIRECTORY, ID_SHMEM);

	/* Primero realizo un wait() en SemFull para verificar que el museo no 
 	 * esté lleno. Luego intento entrar por alguna de las puertas. Dentro 
 	 * del museo espero un tiempo aleatorio y luego salgo
 	 */
	semFull.wait();	
	sprintf(buffer, "PID %d: Pasa semFull\n", getpid());
	write(fileno(stdout), buffer, strlen(buffer));

	nroPuerta = rand() % cantPuertas;
	semDoor.wait(nroPuerta);
	sprintf(buffer, "PID %d: Entra por la puerta N°%d\n", getpid(), nroPuerta);
	write(fileno(stdout), buffer, strlen(buffer));
	// tiempo de espera de entre 100 ms y 300 ms
	usleep((rand() % 201 + 100) * 1000);
	semDoor.signal(nroPuerta);

	semControl.wait();
	shMem.read(&data);
	sprintf(buffer, "PID %d: Accede a la SharedMemory\n", getpid());
	write(fileno(stdout), buffer, strlen(buffer));
	data.cantPersonas++;
	//cpCantPersonas = (*cantPersonas);
	sprintf(buffer, "PID %d: Cantidad de personas en el museo: %d\n", getpid(), 
	data.cantPersonas);
	write(fileno(stdout), buffer, strlen(buffer));
	shMem.write(&data);
	semControl.signal();	

	if (data.cantPersonas != BUFF_MAX)
	{
		sprintf(buffer, "PID %d: El Museo no está lleno, realizo un signal en semFull.\n", getpid());
		write(fileno(stdout), buffer, strlen(buffer));
		semFull.signal();
	}
	else
	{
		sprintf(buffer, "PID %d: Museo Lleno\n", getpid());
		write(fileno(stdout), buffer, strlen(buffer));
	}

	// Tiempo de procesamiento simulado (entre 1 y 10 segundos)
	usleep((rand() % 5001 + 100) * 1000);

	semControl.wait();
	shMem.read(&data);
	sprintf(buffer, "PID %d: Accede a la SharedMemory\n", getpid());
	write(fileno(stdout), buffer, strlen(buffer));
	data.cantPersonas--;
	//cpCantPersonas = (*cantPersonas);
	sprintf(buffer, "PID %d: Cantidad de personas en el museo: %d\n", getpid(), 
	data.cantPersonas);
	write(fileno(stdout), buffer, strlen(buffer));
	shMem.write(&data);
	semControl.signal();
	
	nroPuerta = rand() % cantPuertas;
	semDoor.wait(nroPuerta);
	usleep((rand() % 201 + 100) * 1000);
	sprintf(buffer, "PID %d: Sale por la puerta N°%d\n", getpid(), nroPuerta);
	write(fileno(stdout), buffer, strlen(buffer));
	semDoor.signal(nroPuerta);

	if (data.cantPersonas == (BUFF_MAX -1) )
	{
		// Si el museo estaba lleno, al salir la Persona deja de estarlo, por lo
		// cual el mismo hace un signal sobre semFull
		sprintf(buffer, "PID %d: El Museo estaba lleno, realizo un signal en semFull\n", getpid());
		write(fileno(stdout), buffer, strlen(buffer));
		semFull.signal();
	}
	else
	{
		sprintf(buffer, "PID %d: El museo no estaba lleno, salgo sin hacer nada\n", getpid());
		write(fileno(stdout), buffer, strlen(buffer));
	}
	
	exit(0);
}
