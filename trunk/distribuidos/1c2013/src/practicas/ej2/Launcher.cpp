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

/* Archivo que se encarga de inicializar todos los IPC a utilizar en los
 * procesos Puerta y llamar a los mismos */


/* Definición de variables estáticas para pasar argumentos a los procesos */
static char param[8];
static char buffer[255];


// Función donde inicializo los IPCs
void init_IPCs(void);

int main(int argc, char* argv[])
{
	pid_t pid;

	// Inicializo el entorno
	srand(getpid());
	sprintf(param, "%d", CANT_DOORS);
	init_IPCs();

	// Creo indefinidamente personas. El proceso Terminator 
	// será el encargado de terminar este proceso
	while(1)
	{
		if ((pid = fork()) < 0)
		{
			sprintf(buffer, "Launcher Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
	  }
		else if (pid == 0)
		{
			// Paso como parámetro a Persona el nombre del programa y la 
			// cantidad de puertas del museo
			execlp("./persona", "persona", param, (char*) 0);
			sprintf(buffer, "Launcher Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
		else
		{
			// Espero un tiempo N y luego llamo a otro proceso Persona
			usleep(500 * 1000);
		}
	} 
	return 0;
}

void init_IPCs()
{
	/* Este programa es aquel que debe inicializar a los IPC. En este caso se
 	 * precisan 4 IPCs:
 	 * semControl: Controla el acceso a la memoria compartida
 	 * semFull: Controla que el museo no esté lleno
 	 * semDoor: Sincroniza a los procesos Persona que desean entrar 
 	 * por las puertas
 	 * shMem: Memoria Compartida donde se encontrarán las variables de control 
 	 */
	int i = 0;	
	ControlData data;
	data.cantPersonas = 0;

	Semaphore semControl = Semaphore();	
	semControl.createSemaphore((char *) DIRECTORY, ID_SEMCONTROL, 1);
	semControl.initializeSemaphore(0, 1);

	Semaphore semDoor = Semaphore();
	semDoor.createSemaphore((char *) DIRECTORY, ID_SEMDOOR, CANT_DOORS);
	for (i = 0; i < CANT_DOORS; i++)
	{
		semDoor.initializeSemaphore(i, 1);
	}	

	Semaphore semFull = Semaphore();
	semFull.createSemaphore((char *) DIRECTORY, ID_SEMFULL, 1);
	semFull.initializeSemaphore(0, 1);

	// El tamaño de la memoria compartida es el de un único entero, que posee
	// como valor la cantidad de personas en el museo
	ControlDataSharedMemory shMem = ControlDataSharedMemory();
	shMem.createSharedMemory((char *) DIRECTORY, ID_SHMEM);
	shMem.write(&data);

	sprintf(buffer, "Proceso Launcher - Valor inicial de cantPersonas: %d\n", data.cantPersonas);
	write(fileno(stdout), buffer, strlen(buffer));
}

