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

#include "./IPC/MessageQueue/IntercambioMessageQueue.h"
#include "./IPC/SharedMemory/InfoSalaSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{	
	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida Sala
		Semaphore semControlSala = Semaphore();	
		semControlSala.getSemaphore((char *)DIRECTORY, ID_SEM_CONTROL_SALA, 1);
		semControlSala.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}

	try{
		// Obteniendo semaforo de bloqueo de la puerta
		Semaphore semPuerta = Semaphore();
		semPuerta.getSemaphore((char *)DIRECTORY, ID_SEM_PUERTA, 1);
		semPuerta.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	try{	
		// Obteniendo semaforo de bloqueo de buses
		Semaphore semBuses = Semaphore();
		semBuses.getSemaphore((char *)DIRECTORY, ID_SEM_BUSES, 1);
		semBuses.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	try{
		// Obteniendo la cola de mensajes de intercambio
		IntercambioMessageQueue intercambioMessageQueue = IntercambioMessageQueue();
		intercambioMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_PERSONAS);
		intercambioMessageQueue.destroy();
		}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	try{	
		// Obteniendo memoria compartida Sala
		InfoSalaSharedMemory shMemSala = InfoSalaSharedMemory();
		shMemSala.getSharedMemory((char *)DIRECTORY, ID_SHMEM_SALA);
		shMemSala.destroy();
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}	


	return 0;
}

