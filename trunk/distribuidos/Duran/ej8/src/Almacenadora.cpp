/*
 * Almacenadora.cpp
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
#include <time.h>

#include "./IPC/IPCException.h"
#include "./IPC/Semaphore/Semaphore.h"

#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Almacenadora: Cantidad de parametros invalida.\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int cantBotellas = 0;
	sscanf(argv[1] , "%d", &cantBotellas);

	sprintf(buffer, "Almacenadora: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer));
	
	Semaphore semAlmacenadora = Semaphore();
	Semaphore semEmbotelladora = Semaphore();
		
	try {
		semAlmacenadora.getSemaphore((char *)DIRECTORY, ID_SEM_ALMACENADORA, 1);
		semEmbotelladora.getSemaphore((char *)DIRECTORY, ID_SEM_EMBOTELLADORA, 1);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Almacenadora: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
		
	bool deboSeguir = (cantBotellas > 0);
	
	while (deboSeguir) {
		try {
			srand(time(NULL));
			int delay = (rand() % 3) + 1;
			sprintf(buffer, "Almacenadora: Depositando botella vacia durante %d segundos\n", delay);
			write(fileno(stdout), buffer, strlen(buffer));
			sleep(delay);
			
			semEmbotelladora.signal();
			
			semAlmacenadora.wait();
			
			delay = (rand() % 3) + 1;
			sprintf(buffer, "Almacenadora: Depositando botella en caja durante %d segundos\n", delay);
			write(fileno(stdout), buffer, strlen(buffer));
			sleep(delay);
			
			cantBotellas--;
			deboSeguir = (cantBotellas > 0);
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Almacenadora: %s\n", ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf (buffer, "Almacenadora: Terminando\n");
	write (fileno(stdout),buffer, strlen(buffer));	
	
	return 0;
}
