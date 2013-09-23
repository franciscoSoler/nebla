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
 
#include <stdlib.h>
 
#include "./IPC/SharedMemory/MuestrasSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 1) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida.\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	sprintf(buffer, "Launcher: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControl = Semaphore();	
	Semaphore semProductores = Semaphore();
	Semaphore semAnalizadores = Semaphore();
		
	MuestrasSharedMemory shMem = MuestrasSharedMemory();
	
	try {
		// Creando semaforo de control de acceso a la memoria compartida
		semControl.createSemaphore((char *)DIRECTORY, ID_SEM_CONTROL, 1);
		semControl.initializeSemaphore(0, 1);

		// Creando semaforo de bloqueo de los productores
		semProductores.createSemaphore((char *)DIRECTORY, ID_SEM_PRODUCTORES, CANT_MUESTRAS);
		for (int i = 0; i < CANT_MUESTRAS; ++i) {
			semProductores.initializeSemaphore(i, 1);
		}

		// Creando semaforo de bloqueo de los analizadores
		semAnalizadores.createSemaphore((char *)DIRECTORY, ID_SEM_ANALIZADORES, CANT_ANALIZADORES);
		for (int i = 0; i < CANT_ANALIZADORES; ++i) {
			semAnalizadores.initializeSemaphore(i, 1);
		}
		
		// Creando memoria compartida
		shMem.createSharedMemory((char *)DIRECTORY, ID_SHMEM_MUESTRAS);
		
		Muestras estado;
		for (int i = 0; i < CANT_MUESTRAS; ++i) {
			estado.muestra[i] = VACIO;
			estado.productorBloqueado[i] = false;
		}
		for (int i = 0; i < CANT_ANALIZADORES; ++i) {
			// Indica que no esta bloqueado
			estado.analizadorBloqueado[i] = CANT_MUESTRAS;
		}
		for (int j = 0; j < CANT_ANALIZADORES; ++j) {
			for (int i = 0; i < CANT_MUESTRAS; ++i) {
				// Inicialmente marco como que todas las muestras fueron analizadas por todos los analizadores
				estado.estadoMuestras[j][i] = true;
			}
		}
		semControl.wait(0);
		{
			shMem.writeInfo(&estado);
		}
		semControl.signal(0);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Launcher Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Creando Productores
	for (int i = 0; i < CANT_MUESTRAS; ++i) {
		static char nroProductor[TAM_BUFFER];
		sprintf(nroProductor,"%d",i);
		pid_t pid = fork();
		if (pid == 0) {
			execlp("./productor", "productor", nroProductor, (char *) 0);
			sprintf(buffer, "Launcher Productor: Error: %s\n", strerror(errno));
			write(fileno(stderr), buffer, strlen(buffer));
			exit(-1);
		}
	}

	// Creando Analizadores
	for (int i = 0; i < CANT_ANALIZADORES; ++i) {
		static char nroAnalizador[TAM_BUFFER];
		sprintf(nroAnalizador,"%d",i);
		pid_t pid = fork();
		if (pid == 0) {
			execlp("./analizador", "analizador", nroAnalizador, (char *) 0);
			sprintf(buffer, "Launcher Analizador: Error: %s\n", strerror(errno));
			write(fileno(stderr), buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
