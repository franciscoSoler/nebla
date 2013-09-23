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

#include <stdlib.h>

#include "./IPC/SharedMemory/MuestrasSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida
		Semaphore semControl = Semaphore();	
		semControl.getSemaphore((char *)DIRECTORY, ID_SEM_CONTROL, 1);
		semControl.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}

	try {
		// Obteniendo semaforo de bloqueo de los productores
		Semaphore semProductores = Semaphore();
		semProductores.getSemaphore((char *)DIRECTORY, ID_SEM_PRODUCTORES, CANT_MUESTRAS);
		semProductores.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}

	try {
		// Obteniendo semaforo de bloqueo de los analizadores
		Semaphore semAnalizadores = Semaphore();
		semAnalizadores.getSemaphore((char *)DIRECTORY, ID_SEM_ANALIZADORES, CANT_ANALIZADORES);
		semAnalizadores.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
		
	try{	
		// Obteniendo memoria compartida
		MuestrasSharedMemory shMem = MuestrasSharedMemory();
		shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM_MUESTRAS);
		shMem.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}

	return 0;
}

