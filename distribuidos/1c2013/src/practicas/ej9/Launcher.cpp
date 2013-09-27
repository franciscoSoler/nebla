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


#include "./IPC/SharedMemory/CountingSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"

#define TAM_BUFFER 255

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantProcesos = 0;
	sscanf(argv[1] , "%d", &cantProcesos);
	
	// Creando semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	semControl.createSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	semControl.initializeSemaphore(0, 1);

	// Creando semaforos de control de los procesos
	Semaphore semProductores = Semaphore();
	semProductores.createSemaphore((char *)DIRECTORY, ID_SEMPRODUCTORES, cantProcesos);
	for (int nroSemaforo = 0; nroSemaforo < cantProcesos; nroSemaforo++) {
	 	semProductores.initializeSemaphore(nroSemaforo, 1);
	}
	
	// Creando la memoria compartida
	CountingSharedMemory shMem = CountingSharedMemory();
	shMem.createSharedMemory((char *)DIRECTORY, ID_SHMEM);
	semControl.wait(0);
	{	
		int p = 0;
		shMem.writeInfo(&p);
	}
	semControl.signal(0);
		
	for (int i = 0; i < cantProcesos; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			char nroProductor[TAM_BUFFER];
			sprintf(nroProductor,"%d",i);
						
			execlp("./productor", "productor", nroProductor, argv[1], (char *) 0);
			sprintf(buffer, "Launcher Error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
		//sleep (1);
	}		
	
	sprintf(buffer, "Launcher Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));
			
	return 0;
}
