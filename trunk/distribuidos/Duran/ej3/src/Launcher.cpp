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
#include "./IPC/MessageQueue/PeligrososMessageQueue.h"
#include "./IPC/MessageQueue/TransportadoraMessageQueue.h"
#include "./IPC/MessageQueue/VerificadosMessageQueue.h"
#include "./IPC/SharedMemory/InfoVuelosSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 4) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT VUELOS] [CANT ESTACIONES VERIFICACION] [CANT PERSONAS]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantVuelos = 0;
	sscanf(argv[1] , "%d", &cantVuelos);
		
	int cantEstaciones = 0;
	sscanf(argv[2] , "%d", &cantEstaciones);

	int cantPersonas = 0;
	sscanf(argv[3] , "%d", &cantPersonas);

	sprintf(buffer, "Launcher: Cantidad vuelos: %d Cantidad Estaciones: %d Cantidad Personas: %d\n", cantVuelos, cantEstaciones, cantPersonas);
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControl = Semaphore();	
	IntercambioMessageQueue intercambioMessageQueue = IntercambioMessageQueue();
	PeligrososMessageQueue peligrososMessageQueue = PeligrososMessageQueue();
	TransportadoraMessageQueue transportadoraMessageQueue = TransportadoraMessageQueue();
	VerificadosMessageQueue verificadosMessageQueue = VerificadosMessageQueue();
	InfoVuelosSharedMemory shMem = InfoVuelosSharedMemory();
	
	try {
		// Creando semaforo de control de acceso a la memoria compartida
		semControl.createSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
		semControl.initializeSemaphore(0, 1);
	
		// Creando la cola de mensajes de intercambio
		intercambioMessageQueue.create((char*)DIRECTORY, ID_COLA_INTERCAMBIO);

		// Creando la cola de mensajes de peligrosos
		peligrososMessageQueue.create((char*)DIRECTORY, ID_COLA_PELIGROSOS);
		
		// Creando la cola de mensajes de transportadora
		transportadoraMessageQueue.create((char*)DIRECTORY, ID_COLA_TRANSPORTADORA);
		
		// Creando la cola de mensajes de verificados
		verificadosMessageQueue.create((char*)DIRECTORY, ID_COLA_VERIFICADOS);

		// Creando memoria compartida
		shMem.createSharedMemory((char *)DIRECTORY, ID_SHMEM);
		
		Informacion informacion;
		
		for (int i = 0; i < cantVuelos; ++i) {
			informacion.vuelos[i].cantidad = 0;
			informacion.vuelos[i].listo = false;
		}		
		semControl.wait(0);
		{
			shMem.writeInfo(&informacion);
		}		
		semControl.signal(0);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Launcher Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
		
	// Creando estaciones de check in
	for (int i = 1; i <= cantVuelos; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroVuelo[TAM_BUFFER];
			sprintf(nroVuelo,"%d",i);
			execlp("./estacionCheckIn", "estacionCheckIn", nroVuelo, (char *) 0);
			sprintf(buffer, "Launcher Estacion CheckIn: Error: %s\n", strerror(errno));
			write(fileno(stderr), buffer, strlen(buffer));
		}
	}
	
	// Creando estaciones de verificacion
	for (int i = 1; i <= cantEstaciones; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroEstacion[TAM_BUFFER];
			sprintf(nroEstacion,"%d",i);
			execlp("./estacionVerificacion", "estacionVerificacion", nroEstacion, (char *) 0);
			sprintf(buffer, "Launcher Estacion Verificacion: Error: %s\n", strerror(errno));
			write(fileno(stderr), buffer, strlen(buffer));
		}
	}	
	
	// Creando Robots
	for (int i = 1; i <= cantVuelos; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroVuelo[TAM_BUFFER];
			sprintf(nroVuelo,"%d",i);
			execlp("./robot", "robot", nroVuelo, (char *) 0);
			sprintf(buffer, "Launcher Robot: Error: %s\n", strerror(errno));
			write(fileno(stderr), buffer, strlen(buffer));
		}
	}

	static char cantidadVuelos[TAM_BUFFER];
	sprintf(cantidadVuelos,"%d",cantVuelos);
		
	// Creando estacion de sospechosos
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./estacionSospechosos", "estacionSospechosos", cantidadVuelos, (char *) 0);
		sprintf(buffer, "Launcher Estacion sospechosos: Error: %s\n", strerror(errno));
		write(fileno(stdout), buffer, strlen(buffer));
	}

	// Creando personas
	for (int i = 1; i <= cantPersonas; i++) {
		static char nroPersona[TAM_BUFFER];
		sprintf(nroPersona,"%d",i);
		pid_t pid = fork();
		if (pid == 0) {
			execlp("./persona", "persona", nroPersona, cantidadVuelos, (char *) 0);
			sprintf(buffer, "Launcher: Persona Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
		
		srand(time(NULL));
		int delay = (rand() % 10) + 1;
		sprintf(buffer, "Launcher: Durmiendo durante %d\n", delay);
		write(fileno(stdout), buffer, strlen(buffer));
		sleep(delay);
	}
		
	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
