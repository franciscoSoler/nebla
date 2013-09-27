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

#include "./IPC/SharedMemory/SamplesTableSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];
			
	sprintf(buffer, "Launcher: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer));
			
	// Creando los semaforos que controlan el avance de productores y analizadores
	Semaphore semProductores = Semaphore();
	semProductores.createSemaphore((char *)DIRECTORY,ID_SEMPRODUCTORES, CANT_PRODUCTORES);	
	for (int i = 0; i < CANT_PRODUCTORES; ++i) {
		semProductores.initializeSemaphore(i, 1);
	}
	
	Semaphore semAnalizadores =  Semaphore();
	semAnalizadores.createSemaphore((char *)DIRECTORY,ID_SEMANALIZADORES, CANT_ANALIZADORES);	
	for (int j = 0; j < CANT_ANALIZADORES; ++j) {
		semAnalizadores.initializeSemaphore(j, 0);
	}
	
	// Creando el semaforo que controla el acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	semControl.createSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	semControl.initializeSemaphore(0, 1);
	
	// Creando la memoria compartida
	SamplesTableSharedMemory table = SamplesTableSharedMemory();
	table.createSharedMemory((char *)DIRECTORY, ID_SHMEM);
	
	SamplesTable initialTable;

	/*for (int j = 0; j < CANT_PRODUCTORES; ++j) {
		initialTable.toma[j] = false;
	}*/
	
	for (int j = 0; j < CANT_ANALIZADORES; ++j) {
		initialTable.analiza[j] = true;
	}
	
	for (int i = 0; i < CANT_ANALIZADORES; ++i) {
		for (int j = 0; j < CANT_PRODUCTORES; ++j) {
			initialTable.sampleReady[i][j] = false;
		}
	}
	
	semControl.wait();
	{
		table.writeInfo(&initialTable);
	}		
	semControl.signal();
	
	// Creando productores
	for (int i = 0; i < CANT_PRODUCTORES; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			char nroProductor[TAM_BUFFER];
			sprintf(nroProductor,"%d",i);
			execlp("./productor", "productor", nroProductor, (char *) 0);
			sprintf(buffer, "Launcher: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}

	// Creando analizadores
	for (int i = 0; i < CANT_ANALIZADORES; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			char nroAnalizador[TAM_BUFFER];
			sprintf(nroAnalizador,"%d",i);
			execlp("./analizador", "analizador", nroAnalizador, (char *) 0);
			sprintf(buffer, "Launcher: Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}
	
	return 0;
}
