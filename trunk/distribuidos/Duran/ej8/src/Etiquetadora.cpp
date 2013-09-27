/*
 * Etiquetadora.cpp
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
		strcpy (buffer, "Error Etiquetadora: Cantidad de parametros invalida.\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	int cantBotellas = 0;
	sscanf(argv[1] , "%d", &cantBotellas);

	sprintf(buffer, "Etiquetadora: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer));
	
	Semaphore semEtiquetadora = Semaphore();
	Semaphore semTapadora = Semaphore();
		
	try {
		semEtiquetadora.getSemaphore((char *)DIRECTORY, ID_SEM_ETIQUETADORA, 1);
		semTapadora.getSemaphore((char *)DIRECTORY, ID_SEM_TAPADORA, 1);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Etiquetadora: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
		
	bool deboSeguir = (cantBotellas > 0);
	
	while (deboSeguir) {
		try {
			semEtiquetadora.wait();
			
			srand(time(NULL));
			int delay = (rand() % 3) + 1;
			sprintf(buffer, "Etiquetadora: Etiquetando durante %d\n", delay);
			write(fileno(stdout), buffer, strlen(buffer));
			sleep(delay);
			
			cantBotellas--;
			deboSeguir = (cantBotellas > 0);
			
			semTapadora.signal();
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Etiquetadora: %s\n", ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf (buffer, "Etiquetadora: Terminando\n");
	write (fileno(stdout),buffer, strlen(buffer));	
	
	return 0;
}
