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

#include "common.h"

#include "./IPC/MessageQueue/RecipientesMessageQueue.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT PRODUCTORES]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantProdcutores = 0;
	sscanf(argv[1] , "%d", &cantProdcutores);
	
	sprintf(buffer, "Launcher: Productores: %d\n", cantProdcutores);
	write(fileno(stdout), buffer, strlen(buffer));
		
	// Crando cola de comunicacion
	RecipientesMessageQueue	recipientesMessageQueue = RecipientesMessageQueue();
	recipientesMessageQueue.create((char *)DIRECTORY, ID_COLA_RECIPIENTES);

	// Creando consumidores
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./llenador", "llenador", (char *) 0);
		sprintf(buffer, "Launcher: Error lanzando el llenador: %s\n", strerror(errno));
		write(fileno(stdout), buffer, strlen(buffer));
	}

	pid = fork();
	if (pid == 0) {
		execlp("./etiquetador", "etiquetador", (char *) 0);
		sprintf(buffer, "Launcher: Error lanzando el etiquetador: %s\n", strerror(errno));
		write(fileno(stdout), buffer, strlen(buffer));
	}
	
	pid = fork();
	if (pid == 0) {
		execlp("./impresor", "impresor", (char *) 0);
		sprintf(buffer, "Launcher: Error lanzando el impresor: %s\n", strerror(errno));
		write(fileno(stdout), buffer, strlen(buffer));
	}
			
	// Creando productores
	for (int i = 1; i <= cantProdcutores; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			static char nroProductor[TAM_BUFFER];
			sprintf(nroProductor,"%d",i);
			execlp("./productor", "productor", nroProductor, (char *) 0);
			sprintf(buffer, "Launcher: Error lanzando un productor: %s\n", strerror(errno));
			write(fileno(stdout), buffer, strlen(buffer));
			exit(-1);
		}
		
		srand(time(NULL));
		int delay = (rand() % 5) + 1;
		sprintf(buffer, "Launcher: Durmiendo durante %d\n", delay);
		write(fileno(stdout), buffer, strlen(buffer));
		sleep(delay);
	}
	
	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));
				
	return 0;
}
