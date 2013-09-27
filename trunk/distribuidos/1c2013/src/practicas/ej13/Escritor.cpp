#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./IPC/SharedMemory/ControlDataSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

void escribir(void);
// Función que imprime los datos almacenados en al memoria compartida
void imprimirData(ControlData data, int lectorId);


int main(int argc, char* argv[])
{
	char buffer[255];
	int escritorId;	
	sscanf(argv[1], "%d", &escritorId);
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

	// Ciclo de Escritura
	while(1)
	{
		// Ciclo de Pre-Escritura
		semControl.wait();
		sprintf(buffer, "Escritor N°%d: Accede a la SharedMemory. Ciclo Pre-Escritura\n", escritorId);
		write(fileno(stdout), buffer, strlen(buffer));
		shMem.read(&data);
		imprimirData(data, escritorId);

		if (estabaEsperando == true)
		{
			data.cEscritoresEsperando--;
			estabaEsperando = false;

			// Verifico si hay más lectores para liberar
			if (data.cEscritoresEsperando > 0)
			{
				// Libero a otro lector
				sprintf(buffer, "Lector N°%d: Se libera a un lector.\n", escritorId); 
				write(fileno(stdout), buffer, strlen(buffer));
				semEsc.signal();
			}
		}

		if (data.estado == ESCRIBIENDO)
		{
			if (data.cLectoresLeyendo == 0)
			{
				if (data.cEscritoresEscribiendo == 0)
				{
					data.cEscritoresEscribiendo++;
					shMem.write(&data);
					sprintf(buffer, "Escritor N°%d: Libera a la SharedMemory\n", escritorId);
					write(fileno(stdout), buffer, strlen(buffer));
					semControl.signal();	
				}
				else
				{
					shMem.write(&data);
					sprintf(buffer, "Escritor N°%d: Libera a la SharedMemory\n", escritorId);
					write(fileno(stdout), buffer, strlen(buffer));
					semControl.signal();	
					data.cEscritoresEsperando++;
					estabaEsperando = true;
					sprintf(buffer, "Escritor N°%d: Se bloquea\n", escritorId);
					write(fileno(stdout), buffer, strlen(buffer));
					semEsc.wait();
					sprintf(buffer, "Escritor N°%d: Es desbloqueado\n", escritorId);
					write(fileno(stdout), buffer, strlen(buffer));
					continue;
				}
			}
			else
			{
				// Hay lectores leyendo, el proceso se queda esperando hasta que estos 
				// terminen de leer
				data.cEscritoresEsperando++;
				shMem.write(&data);
				sprintf(buffer, "Escritor N°%d: Libera a la SharedMemory\n", escritorId);
				write(fileno(stdout), buffer, strlen(buffer));
				semControl.signal();
				estabaEsperando = true;
				sprintf(buffer, "Escritor N°%d: Se bloquea\n", escritorId);
				write(fileno(stdout), buffer, strlen(buffer));
				semEsc.wait();
				sprintf(buffer, "Escritor N°%d: Es desbloqueado\n", escritorId);
				write(fileno(stdout), buffer, strlen(buffer));
				continue;
			}
		}
		else if (data.estado == LIBRE)
		{
			data.estado = ESCRIBIENDO;
			data.cEscritoresEscribiendo++;
			shMem.write(&data);
			sprintf(buffer, "Escritor N°%d: Libera a la SharedMemory\n", escritorId);
			write(fileno(stdout), buffer, strlen(buffer));
			semControl.signal();
		}
		else
		{
			data.cEscritoresEsperando++;
			shMem.write(&data);
			sprintf(buffer, "Escritor N°%d: Libera a la SharedMemory\n", escritorId);
			write(fileno(stdout), buffer, strlen(buffer));
			semControl.signal();
			estabaEsperando = true;
			sprintf(buffer, "Escritor N°%d: Se bloquea\n", escritorId);
			write(fileno(stdout), buffer, strlen(buffer));
			semEsc.wait();
			sprintf(buffer, "Escritor N°%d: Es desbloqueado\n", escritorId);
			write(fileno(stdout), buffer, strlen(buffer));
			continue;
		}	

		sprintf(buffer, "Escritor N°%d: Procede a escribir\n", escritorId);
		write(fileno(stdout), buffer, strlen(buffer));
		escribir();

		// Ciclo de Post-Escritura
		semControl.wait();
		sprintf(buffer, "Escritor N°%d: Accede a SharedMemory. Ciclo Post-Escritura\n", escritorId);
		write(fileno(stdout), buffer, strlen(buffer));
		shMem.read(&data);
		imprimirData(data, escritorId);	
		data.cEscritoresEscribiendo--;
		if (data.cEscritoresEsperando == 0)
		{
			if (data.estado == ESCRIBIENDO)
			{
				data.estado = LIBRE;	
			}

			if (data.cLectoresEsperando > 0)
			{
				sprintf(buffer, "Escritor N°%d: Libera a lector\n", escritorId);
				write(fileno(stdout), buffer, strlen(buffer));
				semLec.signal();
			}	
		}

		shMem.write(&data);
		sprintf(buffer, "Escritor N°%d: Libera SharedMemory\n", escritorId);
		write(fileno(stdout), buffer, strlen(buffer));
		semControl.signal();

		// Luego de un ciclo completo, pongo al escritor a dormir un tiempo 
		usleep(((rand() % 1401 + 100)) * 1000);
	}
	exit(0);
}

void escribir()
{
	// Simulo el tiempo que los lectores se quedan leyendo entre 0.1 y 1 segundos
	usleep(((rand() % 901 + 100)) * 1000);
}

void imprimirData(ControlData data, int escritorId)
{
	char estadoValues[][30] = {"LIBRE", "LEYENDO", "ESCRIBIENDO"};
	char buffer[100];

	sprintf(buffer, "Escritor N°%d: Estado - %s\n", escritorId, estadoValues[data.estado]);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Escritor N°%d: Lectores Leyendo - %d\n", escritorId, data.cLectoresLeyendo);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Escritor N°%d: Lectores Esperando - %d\n", escritorId, data.cLectoresEsperando);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Escritor N°%d: Escritores Escribiendo - %d\n", escritorId, data.cEscritoresEscribiendo);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Escritor N°%d: Escritores Esperando - %d\n", escritorId, data.cEscritoresEsperando);
	write(fileno(stdout), buffer, strlen(buffer));
	sprintf(buffer, "Escritor N°%d: Accesos - %d\n", escritorId, data.cAccesos);
	write(fileno(stdout), buffer, strlen(buffer));
}
