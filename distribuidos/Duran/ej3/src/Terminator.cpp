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
	try {
		// Obteniendo memoria compartida
		InfoVuelosSharedMemory shMem = InfoVuelosSharedMemory();
		shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
		shMem.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
		
	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida
		Semaphore semControl = Semaphore();	
		semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
		semControl.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
			
	try {
		// Obteniendo la cola de mensajes de intercambio
		IntercambioMessageQueue intercambioMessageQueue = IntercambioMessageQueue();
		intercambioMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_INTERCAMBIO);
		intercambioMessageQueue.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	try {
		// Obteniendo la cola de mensajes de peligrosos
		PeligrososMessageQueue peligrososMessageQueue = PeligrososMessageQueue();
		peligrososMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_PELIGROSOS);
		peligrososMessageQueue.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
		
	try {	
		// Obteniendo la cola de mensajes de transportadora
		TransportadoraMessageQueue transportadoraMessageQueue = TransportadoraMessageQueue();
		transportadoraMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_TRANSPORTADORA);
		transportadoraMessageQueue.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	try {
		// Obteniendo la cola de mensajes de verificados
		VerificadosMessageQueue verificadosMessageQueue = VerificadosMessageQueue();
		verificadosMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_VERIFICADOS);
		verificadosMessageQueue.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	return 0;
}

