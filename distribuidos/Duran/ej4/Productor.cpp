/*
 * Productor.cpp
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

#include "ControladorProductor.h"

#define TAM_BUFFER 255

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Productor: Cantidad de parametros invalida\n\tUso:\n./productor [NRO PRODUCTOR]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroProductor = 0;
	sscanf(argv[1] , "%d", &nroProductor);
	
	sprintf(buffer, "Productor: %d Iniciando\n", nroProductor);
	write(fileno(stdout), buffer, strlen(buffer));

	ControladorProductor controlador = ControladorProductor();
	bool deboProducir = true;
	int cantidad = 0;
	
	while (deboProducir) {
		
		int producir = rand() % 15 + 1;
		sprintf (buffer, "Productor: %d Produciento durante %d segundos\n", nroProductor, producir);
		write (fileno(stdout),buffer, strlen(buffer));
		usleep(producir*1000*1000);
		
		Recipiente recipiente;
		recipiente.idProductor = nroProductor;
		recipiente.idRecipiente = cantidad++;
		deboProducir = controlador.depositarRecipiente(recipiente);
	}
		
	sprintf(buffer, "Productor: %d Terminado \n", nroProductor);
	write(fileno(stdout), buffer, strlen(buffer));
	
	return 0;
}
