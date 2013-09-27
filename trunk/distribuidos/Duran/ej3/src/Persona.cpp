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
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		sprintf(buffer, "Perssona: Error: Cantidad de parametros invalida\n Uso:\n ./persona [ID PERSONA] [CANT VUELOS]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int nroPersona;
	sscanf(argv[1] , "%d", &nroPersona);
	
	int cantidadVuelos;
	sscanf(argv[2] , "%d", &cantidadVuelos);
	
	ControladorPersona controlador = ControladorPersona(nroPersona);
	
	srand(time(NULL));
	int nroVuelo = rand() % cantidadVuelos + 1;
	
	int tam = rand() % 100 + 1;
	int contenido = rand() % 4;
	
	Equipaje equipaje;
	equipaje.size = tam;
	equipaje.contenido = (Contenido)contenido;
	
	controlador.enviarEquipaje(equipaje, nroVuelo);
	
	return 0;
}
