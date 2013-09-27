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

#include "./IPC/SharedMemory/InfoMuseoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT PUERTAS] [CANT MAXIMA]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantPuertas = 0;
	sscanf(argv[1] , "%d", &cantPuertas);

	int cantMaxima = 0;
	sscanf(argv[2] , "%d", &cantMaxima);
		
	sprintf(buffer, "Launcher: Puertas: %d Cantidad maxima: %d\n", cantPuertas, cantMaxima);
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
	
	InfoMuseo initialInfo;
	initialInfo.abierto = false;
	initialInfo.cantidad = 0;
	initialInfo.total = 0;
	initialInfo.cantidadMaxima = cantMaxima;
		
	semControl.wait(0);
	{
		shMem.writeInfo(&initialInfo);
	}		
	semControl.signal(0);
	
	// Creando puertas entrada
	for (int i = 1; i <= cantPuertas; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroPuerta[TAM_BUFFER];
			sprintf(nroPuerta,"%d",i);
			execlp("./puertaEntrada", "puertaEntrada", nroPuerta, (char *) 0);
			sprintf(buffer, "Launcher Puerta Entrada: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
		
		srand(time(NULL));
		int delay = (rand() % 2) + 1;
		sprintf(buffer, "Launcher: Durmiendo durante %d\n", delay);
		write(fileno(stdout), buffer, strlen(buffer));
		sleep(delay);
	}
	
	// Creando puertas salida
	for (int i = 1; i <= cantPuertas; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroPuerta[TAM_BUFFER];
			sprintf(nroPuerta,"%d",i);
			execlp("./puertaSalida", "puertaSalida", nroPuerta, (char *) 0);
			sprintf(buffer, "Launcher Puerta Salida: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
		
		srand(time(NULL));
		int delay = (rand() % 2) + 1;
		sprintf(buffer, "Launcher: Durmiendo durante %d\n", delay);
		write(fileno(stdout), buffer, strlen(buffer));
		sleep(delay);
	}
	
	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
