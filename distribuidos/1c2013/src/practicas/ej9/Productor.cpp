/*
 * Productos.cpp
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

#include "Buffer.h"

#define TAM_BUFFER 255
#define TIEMPO_PRODUCIENDO 3
#define CANT_A_PRODUCIR 10

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		strcpy (buffer, "Error Productor: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroProductor = 0;
	sscanf(argv[1] , "%d", &nroProductor);

	int cantProductor = 0;
	sscanf(argv[2] , "%d", &cantProductor);	
	
	Buffer bufferProductos (cantProductor);

	srand(getpid());
	
	int cantProducida = 0;
	bool deboSeguir = true;
	while ((cantProducida < CANT_A_PRODUCIR) && deboSeguir)
	{
		int tiempo = (rand() % TIEMPO_PRODUCIENDO) + 1;
		sprintf(buffer, "Productor %d produciendo durante %d \n", nroProductor, tiempo);	
		write (fileno(stdout), buffer, strlen(buffer));
		sleep(tiempo);
		
		deboSeguir = bufferProductos.producir(nroProductor);
		cantProducida++;
	
	}	
	
	sprintf(buffer, "Productor %d termino \n", nroProductor);
	write (fileno(stdout), buffer, strlen(buffer));

	return 0;
}

