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

#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT BOTELLAS]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantBotellas = 0;
	sscanf(argv[1] , "%d", &cantBotellas);
		
	sprintf(buffer, "Launcher: Cantidad botellas: %d \n", cantBotellas);
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semEmbotelladora = Semaphore();
	Semaphore semEtiquetadora = Semaphore();
	Semaphore semTapadora = Semaphore();
	Semaphore semAlmacenadora = Semaphore();

	try {
		semEmbotelladora.createSemaphore((char *)DIRECTORY, ID_SEM_EMBOTELLADORA, 1);
		semEmbotelladora.initializeSemaphore(0, 0);
		semEtiquetadora.createSemaphore((char *)DIRECTORY, ID_SEM_ETIQUETADORA, 1);
		semEtiquetadora.initializeSemaphore(0, 0);
		semTapadora.createSemaphore((char *)DIRECTORY, ID_SEM_TAPADORA, 1);
		semTapadora.initializeSemaphore(0, 0);
		semAlmacenadora.createSemaphore((char *)DIRECTORY, ID_SEM_ALMACENADORA, 1);
		semAlmacenadora.initializeSemaphore(0, 0);		
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Launcher Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	static char cantidadBotellas[TAM_BUFFER];
	sprintf(cantidadBotellas,"%d",cantBotellas);
		
	// Creando Embotelladora
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./embotelladora", "embotelladora", cantidadBotellas,(char *) 0);
		sprintf(buffer, "Launcher : Error: %s\n", strerror(errno));
		write(fileno(stderr), buffer, strlen(buffer));
		exit(-1);
	}

	// Creando Etiquetadora
	pid = fork();
	if (pid == 0) {
		execlp("./etiquetadora", "etiquetadora", cantidadBotellas,(char *) 0);
		sprintf(buffer, "Launcher : Error: %s\n", strerror(errno));
		write(fileno(stderr), buffer, strlen(buffer));
		exit(-1);
	}
	
	// Creando Tapadora
	pid = fork();
	if (pid == 0) {
		execlp("./tapadora", "tapadora", cantidadBotellas,(char *) 0);
		sprintf(buffer, "Launcher : Error: %s\n", strerror(errno));
		write(fileno(stderr), buffer, strlen(buffer));
		exit(-1);
	}

	// Creando Almacenadora
	pid = fork();
	if (pid == 0) {
		execlp("./almacenadora", "almacenadora", cantidadBotellas,(char *) 0);
		sprintf(buffer, "Launcher : Error: %s\n", strerror(errno));
		write(fileno(stderr), buffer, strlen(buffer));
		exit(-1);
	}	

	sprintf(buffer, "Launcher: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
