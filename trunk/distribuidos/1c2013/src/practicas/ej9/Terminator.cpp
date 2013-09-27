/*
 * terminator.cpp
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

#include "./IPC/SharedMemory/CountingSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"

int main(int argc, char **argv)
{
	char buffer[255];

	if (argc != 2) {
		strcpy (buffer, "Error: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantProcesos = 0;
	sscanf(argv[1] , "%d", &cantProcesos);
	
	
	// Obteniendo semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	semControl.destroy();

	// Obteniendo semaforos de control de los procesos
	Semaphore semProductores = Semaphore();
	semProductores.getSemaphore((char *)DIRECTORY, ID_SEMPRODUCTORES, cantProcesos);
	semProductores.destroy();
	
	// Obteniendo la memoria compartida	
	CountingSharedMemory shMem = CountingSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	shMem.destroy();
	
	return 0;
}

