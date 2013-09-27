#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./IPC/SharedMemory/ControlDataSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

void leer(void);
// Función que imprime los datos almacenados en al memoria compartida
void imprimirData(ControlData data, int lectorId);


int main(int argc, char* argv[])
{
	char buffer[255];
	int lectorId;	
	sscanf(argv[1], "%d", &lectorId);
	ControlData data;	
	bool estabaEsperando = false;
	
	// Le doy al generador números Pseudoaleatorios el pid del proceso
	srand(getpid());	

	// Inicializo los IPCs
	Semaphore semControl = Semaphore();
	semControl.getSemaphore((char *) DIRECTORY, ID_SEMCONTROL, 0);
	Semaphore semLec = Semaphore();
	semLec.getSemaphore((char *) DIRECTORY, ID_SEMLEC, 0);
	Semaphore semEsc = Semaphore();
	semEsc.getSemaphore((char *) DIRECTORY, ID_SEMESC, 0);
	ControlDataSharedMemory shMem = ControlDataSharedMemory();
	shMem.getSharedMemory((char *) DIRECTORY, ID_SHMEM);

	// Ciclo de Lectura 
	while(1)
	{
		// Ciclo de Pre-Lectura
		semControl.wait();
		sprintf(buffer, "Lector N°%d: Accede a la SharedMemory. Pre-Lectura\n", lectorId);
		write(fileno(stdout), buffer, strlen(buffer));
		shMem.read(&data);
		imprimirData(data, lectorId);

		if (estabaEsperando == true)
		{
			data.cLectoresEsperando--;
			estabaEsperando = false;
	
			// Verifico si hay más lectores para liberar
			if (data.cLectoresEsperando > 0)
			{
				// Libero a otro lector
				sprintf(buffer, "Lector N°%d: Se libera a un lector.\n", lectorId); 
				write(fileno(stdout), buffer, strlen(buffer));
				semLec.signal();
			}
		}

		if (data.estado == LEYENDO || data.estado == LIBRE)
		{
			data.cLectoresLeyendo++;
			data.cAccesos++;

			if (data.cAccesos == MAX_ACCESOS)
			{	
				data.estado = ESCRIBIENDO;	
				data.cAccesos = 0;
				if (data.cEscritoresEsperando > 0 && data.cLectoresLeyendo == 0)
				{
					sprintf(buffer, "Lector N°%d: Se libera a un escritor.\n", lectorId); 
					write(fileno(stdout), buffer, strlen(buffer));
					semEsc.signal();
				}
				sprintf(buffer, "Lector N°%d: Libera SharedMemory\n", lectorId);
				write(fileno(stdout), buffer, strlen(buffer));
				shMem.write(&data);
				semControl.signal();
			}
			else
			{
				if (data.estado == LIBRE)
				{
					data.estado = LEYENDO;
				}
				sprintf(buffer, "Lector N°%d: Libera SharedMemory\n", lectorId);
				write(fileno(stdout), buffer, strlen(buffer));
				shMem.write(&data);
				semControl.signal();
			}
		}
		else 
		{
			data.cLectoresEsperando++;
			estabaEsperando = true;
			shMem.write(&data);
			semControl.signal();
			sprintf(buffer, "Lector N°%d: Lector se bloquea\n", lectorId);
			write(fileno(stdout), buffer, strlen(buffer));
			semLec.wait();
			sprintf(buffer, "Lector N°%d: Lector desbloqueado\n", lectorId);
			write(fileno(stdout), buffer, strlen(buffer));
			continue;
		}			
		
		sprintf(buffer, "Lector N°%d: Lector procede a leer\n", lectorId);
		write(fileno(stdout), buffer, strlen(buffer));
		// Se pone un método equivalente a la lectura
		leer();

		// Ciclo de Post-Lectura
		semControl.wait();
		sprintf(buffer, "Lector N°%d: Accede a SharedMemory. Ciclo Post-Lectura\n", lectorId);
		write(fileno(stdout), buffer, strlen(buffer));
		shMem.read(&data);
		imprimirData(data, lectorId);
		data.cLectoresLeyendo--;
		if (data.cLectoresLeyendo	== 0)
		{
			if ( data.estado == LEYENDO)
			{
				data.estado = LIBRE;
			}
			else if (data.estado == ESCRIBIENDO && data.cEscritoresEsperando > 0)
			{
				sprintf(buffer, "Lector N°%d: Se libera a un escritor.\n", lectorId); 
				write(fileno(stdout), buffer, strlen(buffer));
				semEsc.signal();
			}
		}
		shMem.write(&data);
		sprintf(buffer, "Lector N°%d: Libera SharedMemory\n", lectorId);
		write(fileno(stdout), buffer, strlen(buffer));
		semControl.signal();

		// Luego de un ciclo completo, el Lector procede a dormir un tiempo 
		usleep(((rand() % 901 + 100)) * 1000);
	}

	exit(0);
}

void leer()
{
	// Simulo el tiempo que los lectores se quedan leyendo entre 0.1 y 1 segundos
	usleep(((rand() % 901 + 100)) * 1000);
}	

void imprimirData(ControlData data, int lectorId)
{
	char estadoValues[][30] = {"LIBRE", "LEYENDO", "ESCRIBIENDO"};
	char buffer[100];

	sprintf(buffer, "Lector N°%d: Estado - %s\n", lectorId, estadoValues[data.estado]);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Lector N°%d: Lectores Leyendo - %d\n", lectorId, data.cLectoresLeyendo);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Lector N°%d: Lectores Esperando - %d\n", lectorId, data.cLectoresEsperando);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Lector N°%d: Escritores Escribiendo - %d\n", lectorId, data.cEscritoresEscribiendo);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Lector N°%d: Escritores Esperando - %d\n", lectorId, data.cEscritoresEsperando);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Lector N°%d: Accesos - %d\n", lectorId, data.cAccesos);
	write(fileno(stdout), buffer, strlen(buffer));
}
