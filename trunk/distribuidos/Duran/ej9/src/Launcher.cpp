/*
 * Launcher.cpp
 * 
 * Copyright 2013 Adrian Duran <nebla@nebla>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
#include "./IPC/SharedMemory/BufferSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT PROCESADORES]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	int cantProcesadores = 0;
	sscanf(argv[1] , "%d", &cantProcesadores);
	
	if (cantProcesadores > MAX_PROCESADORES) {
		sprintf (buffer, "Launcher: Error: Cantidad de procesadores debe ser menor que %d\n", MAX_PROCESADORES);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	sprintf(buffer, "Launcher: Cantidad procesadores: %d\n", cantProcesadores);
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControl = Semaphore();	
	Semaphore semProcesadores = Semaphore();
	
	BufferSharedMemory shMemBuffer = BufferSharedMemory();
	
	try {
		// Creando semaforo de control de acceso a la memoria compartida
		semControl.createSemaphore((char *)DIRECTORY, ID_SEM_CONTROL_BUFFER, 1);
		semControl.initializeSemaphore(0, 1);

		// Creando semaforo de bloqueo de los procesadores
		semProcesadores.createSemaphore((char *)DIRECTORY, ID_SEM_PROCESADORES, cantProcesadores);
		for (int i = 0; i < cantProcesadores; ++i) {
			semProcesadores.initializeSemaphore(i, 1);
		}
		
		// Creando memoria compartida Buffer
		shMemBuffer.createSharedMemory((char *)DIRECTORY, ID_SHMEM_BUFFER);
		
		Buffer informacion;
		informacion.contador = 0;
		for (int i = 0; i < cantProcesadores; ++i) {
			informacion.informacion[i] = 0;
		}
			
		semControl.wait(0);
		{
			shMemBuffer.writeInfo(&informacion);
		}		
		semControl.signal(0);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Launcher Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Creando Procesadores
	static char cantidadProcesadores[TAM_BUFFER];
	sprintf(cantidadProcesadores, "%d", cantProcesadores);
	for (int i = 0; i < cantProcesadores; i++) {
		static char nroProcesador[TAM_BUFFER];
		sprintf(nroProcesador,"%d",i);
		pid_t pid = fork();
		if (pid == 0) {
			execlp("./procesador", "procesador", nroProcesador, cantidadProcesadores, (char *) 0);
			sprintf(buffer, "Launcher Procesador: Error: %s\n", strerror(errno));
			write(fileno(stderr), buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
