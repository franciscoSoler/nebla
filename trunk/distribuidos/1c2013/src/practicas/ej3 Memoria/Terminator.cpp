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
#include "./IPC/SharedMemory/NumeradorSharedMemory.h"

#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"
#include "./Numerador/commonNumerador.h"

#include "./IPC/MessageQueue/ComprarMessageQueue.h"
#include "./IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "./IPC/MessageQueue/VenderMessageQueue.h"


int main(int argc, char **argv)
{	
	Semaphore semControl = Semaphore();
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	semControl.destroy();

	// Obteniendo memoria compartida
	TicketsInfoSharedMemory shMem = TicketsInfoSharedMemory();
	shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	shMem.destroy();

	// Obteniendo colas de mensajes
	GetVendedorMessageQueue	getVendedorQueue1 = GetVendedorMessageQueue();
	getVendedorQueue1.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR_1);
	getVendedorQueue1.destroy();
	
	ComprarMessageQueue comprarQueue1 = ComprarMessageQueue();
	comprarQueue1.getMessageQueue((char*)DIRECTORY, ID_COMPRAR_1);
	comprarQueue1.destroy();

	VenderMessageQueue venderQueue1 = VenderMessageQueue();
	venderQueue1.getMessageQueue((char*)DIRECTORY, ID_VENDER_1);
	venderQueue1.destroy();
	
	GetVendedorMessageQueue	getVendedorQueue2 = GetVendedorMessageQueue();
	getVendedorQueue2.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR_2);
	getVendedorQueue2.destroy();
	
	ComprarMessageQueue comprarQueue2 = ComprarMessageQueue();
	comprarQueue2.getMessageQueue((char*)DIRECTORY, ID_COMPRAR_2);
	comprarQueue2.destroy();

	VenderMessageQueue venderQueue2 = VenderMessageQueue();
	venderQueue2.getMessageQueue((char*)DIRECTORY, ID_VENDER_2);
	venderQueue2.destroy();
	
	// Obteniendo memoria compartida de numerador
	NumeradorSharedMemory shMemNum = NumeradorSharedMemory();
	shMemNum.getSharedMemory((char *)DIRECTORY_NUM, ID_SHMEM_NUMERADOR);
	shMemNum.destroy();
	
	Semaphore semControlNum = Semaphore();
	semControlNum.getSemaphore((char *)DIRECTORY_NUM, ID_SEMCONTROL_NUMERADOR, 1);
	semControlNum.destroy();
	
	return 0;
}

