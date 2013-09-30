/*
 * Persona.cpp
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

#include "ControladorPersona.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		sprintf(buffer, "Perssona: Error: Cantidad de parametros invalida\n Uso:\n ./persona [ID PERSONA] [CANT PUERTAS]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int nroPersona;
	sscanf(argv[1] , "%d", &nroPersona);
	
	int cantidadPuertas;
	sscanf(argv[2] , "%d", &cantidadPuertas);
	
	ControladorPersona controlador = ControladorPersona(nroPersona);
	
	srand(time(NULL));
	int puertaEntrada = rand() % cantidadPuertas + 1;
	
	if (! controlador.entrar(puertaEntrada)) {
		sprintf(buffer, "Persona: %d Quiere entrar por puerta %d pero el museo esta CERRADO\n", nroPersona, puertaEntrada);
		write (fileno(stdout),buffer, strlen(buffer));
	}
	else {
		int paseo = rand() % 10 + 1;
		sprintf(buffer, "Persona: %d Entra por puerta %d y recorre durante %d segundos\n", nroPersona, puertaEntrada, paseo);
		write (fileno(stdout),buffer, strlen(buffer));
		usleep(paseo*1000*1000);
		
		int puertaSalida = rand() % cantidadPuertas + 1;	
		sprintf(buffer, "Persona: %d Sale por puerta %d\n", nroPersona, puertaSalida);
		write (fileno(stdout),buffer, strlen(buffer));
		
		controlador.salir(puertaSalida);
	}
	
	return 0;
}
