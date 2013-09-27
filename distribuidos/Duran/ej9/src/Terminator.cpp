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

#include "./IPC/SharedMemory/BufferSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{			
	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida
		Semaphore semControl = Semaphore();	
		semControl.getSemaphore((char *)DIRECTORY, ID_SEM_CONTROL_BUFFER, 1);
		semControl.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}

	try{
		// Obteniendo semaforo de bloqueo de los procesos
		Semaphore semProcesadores = Semaphore();
		semProcesadores.getSemaphore((char *)DIRECTORY, ID_SEM_PROCESADORES, 1);
		semProcesadores.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	try{	
		// Obteniendo memoria compartida
		BufferSharedMemory shMemBuffer = BufferSharedMemory();
		shMemBuffer.getSharedMemory((char *)DIRECTORY, ID_SHMEM_BUFFER);
		shMemBuffer.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}

	return 0;
}

