/*
 * Administrador.cpp
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

#include "ControladorAdministrador.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Administrador: Error: Cantidad de parametros invalida\n Uso:\n ./administrador [A(brir)/C(errar)]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	char opcion;
	sscanf(argv[1] , "%c", &opcion);
	
	if (opcion != 'A' && opcion != 'a' && opcion != 'C' && opcion != 'c') {
		strcpy (buffer, "Administrador: Error: Parametros invalidos\n Uso:\n ./administrador [A(brir)/C(errar)]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	ControladorAdministrador administrador;
	
	if (opcion == 'A' || opcion == 'a') {
		if (! administrador.abrirMuseo()) {
			strcpy (buffer, "Administrador: Error: El museo ya estaba abierto\n");
			write (fileno(stderr),buffer, strlen(buffer));			
		}
	}
	else {
		if (!administrador.cerrarMuseo()) {
			strcpy (buffer, "Administrador: Error: El museo ya estaba cerrado\n");
			write (fileno(stderr),buffer, strlen(buffer));			
		}			
	}
	return 0;
}
