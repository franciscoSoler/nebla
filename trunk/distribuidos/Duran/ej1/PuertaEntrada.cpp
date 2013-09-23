/*
 * PuertaEntrada.cpp
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "./IPC/SharedMemory/InfoMuseoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"

#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Puerta Entrada: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	srand (time(NULL));

	int totalPuerta = 0;

	int nroPuerta = 0;
	sscanf(argv[1] , "%d", &nroPuerta);
	
	sprintf(buffer, "Puerta Entrada: %d Iniciando\n", nroPuerta);
	write(fileno(stdout), buffer, strlen(buffer));
	
	// Creando semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	if (semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1) != 0) {
		sprintf (buffer, "Puerta Entrada: %d Fallo al obtener el semaforo de control\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Obteniendo memoria compartida
	InfoMuseoSharedMemory shMem = InfoMuseoSharedMemory();
	if (shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM) != 0) {
		sprintf (buffer, "Puerta Entrada: %d Fallo al obtener la memoria\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	bool deboSeguir = true;
	
	while (deboSeguir) {
		
		int dormir = rand() % 5 + 1;
		sprintf (buffer, "Puerta Entrada: %d Durmiendo por %d segundos\n", nroPuerta, dormir);
		write (fileno(stdout),buffer, strlen(buffer));
		usleep(dormir*1000*1000);
		
		totalPuerta++;
		
		sprintf (buffer, "Puerta Entrada: %d Llego la persona %d\n", nroPuerta, totalPuerta);
		write (fileno(stdout),buffer, strlen(buffer));
		
		int semaphoreOperation = semControl.wait(0);
		if (semaphoreOperation == -1) exit(-1);
		{
			InfoMuseo *info =  shMem.readInfo();	
			if (! (*info).abierto) {
				sprintf (buffer, "Puerta Entrada: %d El museo esta CERRADO, la persona %d debe irse\n", nroPuerta,totalPuerta);
				write (fileno(stdout),buffer, strlen(buffer));
			}
			else {
				if ((*info).cantidad == (*info).cantidadMaxima) {
					sprintf (buffer, "Puerta Entrada: %d El museo esta LLENO, la persona %d debe irse\n", nroPuerta,totalPuerta);
					write (fileno(stdout),buffer, strlen(buffer));					
				}
				else {
					sprintf (buffer, "Puerta Entrada: %d La persona %d ingresa al museo\n", nroPuerta,totalPuerta);
					write (fileno(stdout),buffer, strlen(buffer));

					(*info).cantidad++;
					(*info).total++;	
					shMem.writeInfo(info);
				}
			}			
		}
		semControl.signal(0);
		if (semaphoreOperation == -1) exit(-1);
	}
	
	return 0;
}
