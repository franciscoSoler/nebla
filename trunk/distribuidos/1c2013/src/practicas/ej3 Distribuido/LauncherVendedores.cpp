/*
 * LauncherVendedores.cpp
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

#include "./IPC/SharedMemory/TicketsInfoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

#include "./IPC/MessageQueue/ComprarMessageQueue.h"
#include "./IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "./IPC/MessageQueue/VenderMessageQueue.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT VENDEDORES]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantVendedores = 0;
	sscanf(argv[1] , "%d", &cantVendedores);
		
	sprintf(buffer, "Launcher: Vendedores: %d\n", cantVendedores);
	write(fileno(stdout), buffer, strlen(buffer));
		
	// Creando semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	semControl.createSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	semControl.initializeSemaphore(0, 1);
	
	// Creando memoria compartida
	TicketsInfoSharedMemory shMem = TicketsInfoSharedMemory();
	shMem.createSharedMemory((char *)DIRECTORY, ID_SHMEM);
	
	TicketsInfo initialInfo;
	initialInfo.cantidad = 50;
	initialInfo.dinero = 10;
		
	semControl.wait(0);
	{
		shMem.writeInfo(&initialInfo);
	}		
	semControl.signal(0);
	
	// Crando colas de comunicacion
	GetVendedorMessageQueue	getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.create((char *)DIRECTORY, ID_GET_VENDEDOR_1);
	
	ComprarMessageQueue comprarQueue = ComprarMessageQueue();
	comprarQueue.create((char *)DIRECTORY, ID_COMPRAR_1);

	VenderMessageQueue venderQueue = VenderMessageQueue();
	venderQueue.create((char *)DIRECTORY, ID_VENDER_1);
	
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./servidorVendedorEntrada", "servidorVendedorEntrada", (char *) 0);
		sprintf(buffer, "Launcher Vendedores: Execlp ./servidorVendedorEntrada error: %s\n", strerror(errno));
		write(fileno(stdout), buffer, strlen(buffer));
	}
	
	pid_t pid2 = fork();
	if (pid2 == 0) {
		execlp("./servidorVendedorSalida", "servidorVendedorSalida", (char *) 0);
		sprintf(buffer, "Launcher Vendedores: Execlp ./servidorVendedorSalida error: %s\n", strerror(errno));
		write(fileno(stdout), buffer, strlen(buffer));
	}

	// Creando vendedores
	for (int i = 1; i <= cantVendedores; i++) {
		static char nroVendedor[TAM_BUFFER];
		sprintf(nroVendedor,"%d",i);
		pid_t pid = fork();
		if (pid == 0) {
			execlp("./vendedor", "vendedor", nroVendedor, (char *) 0);
			sprintf(buffer, "Launcher Vendedores: Execlp ./vendedor error: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
		}
	}

	sprintf(buffer, "Launcher Vendedores: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));
			
	return 0;
}
