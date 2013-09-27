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

#include "./IPC/MessageQueue/EntradaMessageQueue.h"
#include "./IPC/MessageQueue/RespuestaMessageQueue.h"
#include "./IPC/MessageQueue/SalidaMessageQueue.h"
#include "./IPC/SharedMemory/InfoMuseoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 4) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT PUERTAS] [CAPACIDAD MAXIMA] [CANT PERSONAS]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantPuertas = 0;
	sscanf(argv[1] , "%d", &cantPuertas);

	int cantMaxima = 0;
	sscanf(argv[2] , "%d", &cantMaxima);

	int cantPersonas = 0;
	sscanf(argv[3] , "%d", &cantPersonas);
		
	sprintf(buffer, "Launcher: Puertas: %d Capacidad maxima: %d\n", cantPuertas, cantMaxima);
	write(fileno(stdout), buffer, strlen(buffer));
		
	// Creando semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	if (semControl.createSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1) != 0) {
		sprintf(buffer, "Launcher Error: El semaforo de control de acceso ya existe\n");
		write(fileno(stdout), buffer, strlen(buffer));
		exit(-1);
	}
	semControl.initializeSemaphore(0, 1);
	
	// Creando memoria compartida
	InfoMuseoSharedMemory shMem = InfoMuseoSharedMemory();
	if (shMem.createSharedMemory((char *)DIRECTORY, ID_SHMEM) != 0) {
		sprintf(buffer, "Launcher Error: La memoria compartida ya existe\n");
		write(fileno(stdout), buffer, strlen(buffer));
		exit(-1);
	}
	
	// Obteniendo la cola de mensajes de entrada
	EntradaMessageQueue entradaMessageQueue = EntradaMessageQueue();
	if (entradaMessageQueue.create((char*)DIRECTORY, ID_COLA_ENTRADA) != 0) {
		sprintf (buffer, "Launcher Error: Cola de entrada ya existe\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Creando la cola de mensajes de respuesta
	RespuestaMessageQueue respuestaMessageQueue = RespuestaMessageQueue();
	if (respuestaMessageQueue.create((char*)DIRECTORY, ID_COLA_RESPUESTA) != 0) {
		sprintf (buffer, "Launcher Error: Cola de respuesta ya existe\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	// Creando la cola de mensajes de salida
	SalidaMessageQueue salidaMessageQueue = SalidaMessageQueue();
	if (salidaMessageQueue.create((char*)DIRECTORY, ID_COLA_SALIDA) != 0) {
		sprintf (buffer, "Launcher Error: Cola de salida ya existe\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Creando semaforo de bloqueo de puertas
	Semaphore semPuertas = Semaphore();	
	if (semPuertas.createSemaphore((char *)DIRECTORY, ID_SEMPUERTAS, cantPuertas) != 0) {
		sprintf (buffer, "Launche Error: Semaforos de bloqueo de puertas ya existen\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	for (int i = 0; i < cantPuertas; ++i) {
		semControl.initializeSemaphore(i, 1);
	}
	
	InfoMuseo initialInfo;
	initialInfo.abierto = true;
	initialInfo.cantidad = 0;
	initialInfo.cantidadMaxima = cantMaxima;
	for (int i = 0; i < cantPuertas; ++i) {
		initialInfo.puertasBloqueadas[i] = false;
	}
		
	semControl.wait(0);
	{
		shMem.writeInfo(&initialInfo);
	}		
	semControl.signal(0);

	static char cantidadPuertas[TAM_BUFFER];
	sprintf(cantidadPuertas,"%d",cantPuertas);
	
	// Creando puertas entrada
	for (int i = 1; i <= cantPuertas; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroPuerta[TAM_BUFFER];
			sprintf(nroPuerta,"%d",i);
			execlp("./puertaEntrada", "puertaEntrada", nroPuerta, cantidadPuertas, (char *) 0);
			sprintf(buffer, "Launcher Puerta Entrada: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}
	
	// Creando puertas salida
	for (int i = 1; i <= cantPuertas; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroPuerta[TAM_BUFFER];
			sprintf(nroPuerta,"%d",i);
			execlp("./puertaSalida", "puertaSalida", nroPuerta, cantidadPuertas, (char *) 0);
			sprintf(buffer, "Launcher Puerta Salida: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}
	
	// Creando personas
	for (int i = 1; i <= cantPersonas; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroPersona[TAM_BUFFER];
			sprintf(nroPersona,"%d",i);
			execlp("./persona", "persona", nroPersona, cantidadPuertas, (char *) 0);
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
