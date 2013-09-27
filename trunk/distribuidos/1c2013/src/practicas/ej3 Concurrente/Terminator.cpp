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

#include "./IPC/SharedMemory/TicketsInfoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

#include "./IPC/MessageQueue/ComprarMessageQueue.h"
#include "./IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "./IPC/MessageQueue/VenderMessageQueue.h"


int main(int argc, char **argv)
{	
	Semaphore semControl = Semaphore();
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	semControl.destroy();

	// Creando memoria compartida
	TicketsInfoSharedMemory shMem = TicketsInfoSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	shMem.destroy();

	// Obteniendo colas de mensajes
	GetVendedorMessageQueue	getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR);
	getVendedorQueue.destroy();
	
	ComprarMessageQueue comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR);
	comprarQueue.destroy();

	VenderMessageQueue venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER);
	venderQueue.destroy();
	
	
	return 0;
}

