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
 
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "./IPC/MessageQueue/IntercambioMessageQueue.h"
#include "./IPC/SharedMemory/InfoSalaSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT BUSES] [CANT PERSONAS]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	int cantBuses = 0;
	sscanf(argv[1] , "%d", &cantBuses);
	if (cantBuses > MAX_BUSES) {
		sprintf (buffer, "Launcher: Error: Cantidad de buses debe ser menor que %d\n", MAX_BUSES);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	int cantPersonas = 0;
	sscanf(argv[2] , "%d", &cantPersonas);

	sprintf(buffer, "Launcher: Cantidad buses: %d Cantidad Personas: %d\n", cantBuses, cantPersonas);
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControlSala = Semaphore();	
	Semaphore semPuerta = Semaphore();
	Semaphore semBuses = Semaphore();
	IntercambioMessageQueue intercambioMessageQueue = IntercambioMessageQueue();
	InfoSalaSharedMemory shMemSala = InfoSalaSharedMemory();
	
	try {
		// Creando semaforo de control de acceso a la memoria compartida Sala
		semControlSala.createSemaphore((char *)DIRECTORY, ID_SEM_CONTROL_SALA, 1);
		semControlSala.initializeSemaphore(0, 1);

		// Creando semaforo de bloqueo de la puerta
		semPuerta.createSemaphore((char *)DIRECTORY, ID_SEM_PUERTA, 1);
		semPuerta.initializeSemaphore(0, 0);
		
		// Creando semaforo de bloqueo de buses
		semBuses.createSemaphore((char *)DIRECTORY, ID_SEM_BUSES, cantBuses);
		for (int i = 0; i < cantBuses; ++i) {
			semBuses.initializeSemaphore(i, 0);
		}
		
		for (int i = 0; i < cantBuses; ++i) {
			semBuses.signal(i);
			sprintf(buffer, "Launcher: Probando sem bus: %d\n", i);
			write(fileno(stdout), buffer, strlen(buffer));		
			semBuses.wait(i);
			sprintf(buffer, "Launcher 2: Probando sem bus: %d\n", i);
			write(fileno(stdout), buffer, strlen(buffer));
		}
	
		// Creando la cola de mensajes de intercambio
		intercambioMessageQueue.create((char*)DIRECTORY, ID_COLA_PERSONAS);

		// Creando memoria compartida Sala
		shMemSala.createSharedMemory((char *)DIRECTORY, ID_SHMEM_SALA);
		InfoSala informacionSala;
		informacionSala.puertaBloqueada = false;
		informacionSala.busEnSala = false;
		for (int i = 0; i < TAM_SALA; ++i) {
			informacionSala.idPersonas[i] = 0;
		}
		for (int i = 0; i < cantBuses; ++i) {
			informacionSala.busBloqueado[i] = false;
		}	
		semControlSala.wait(0);
		{
			shMemSala.writeInfo(&informacionSala);
		}		
		semControlSala.signal(0);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Launcher Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Creando Buses
	static char cantidadBuses[TAM_BUFFER];
	sprintf(cantidadBuses, "%d",cantBuses);
	for (int i = 0; i < cantBuses; i++) {
		static char nroBus[TAM_BUFFER];
		sprintf(nroBus,"%d",i);
		pid_t pid = fork();
		if (pid == 0) {
			execlp("./bus", "bus", nroBus, cantidadBuses, (char *) 0);
			sprintf(buffer, "Launcher Bus: Error: %s\n", strerror(errno));
			write(fileno(stderr), buffer, strlen(buffer));
		}
	}
	
	// Creando Puerta
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./puerta", "puerta", (char *) 0);
		sprintf(buffer, "Launcher Puerta: Error: %s\n", strerror(errno));
		write(fileno(stderr), buffer, strlen(buffer));
	}

	// Creando Productor de personas
	static char cantidadPersonas[TAM_BUFFER];
	sprintf(cantidadPersonas, "%d",cantPersonas);
	pid = fork();
	if (pid == 0) {
		execlp("./productor", "productor", cantidadPersonas, (char *) 0);
		sprintf(buffer, "Launcher Productor: Error: %s\n", strerror(errno));
		write(fileno(stderr), buffer, strlen(buffer));
	}
	
	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
