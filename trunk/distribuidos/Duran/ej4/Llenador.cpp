/*
 * Llenador.cpp
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

#include "ControladorLlenador.h"

#include "common.h"

#define TAM_BUFFER 255

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	sprintf(buffer, "Llenador: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer));
	
	ControladorLlenador controladorLlenador = ControladorLlenador();
		
	bool deboSeguir = true;
		
	while (deboSeguir) {	
		
		Recipiente recipienteALlenar;
		if (controladorLlenador.obtenerRecipienteParaLlenar(&recipienteALlenar) != 0) {
			sprintf(buffer, "Llenador: Error al recibir el recipiente\n");
			write (fileno(stderr), buffer, strlen(buffer));
			exit(-1);
		}

		sprintf(buffer, "Llenador: Le llego el recipiente %d del productor %d\n", recipienteALlenar.idProductor, recipienteALlenar.idRecipiente);
		write (fileno(stdout), buffer, strlen(buffer));
		
		int llenar = rand() % 2 + 1;
		sprintf(buffer, "Llenador: Llenando durante %d segundos\n", llenar);
		write (fileno(stdout), buffer, strlen(buffer));
		usleep(llenar*1000*1000);
	}
	
	sprintf(buffer, "Llenador: Terminado\n");
	write(fileno(stdout), buffer, strlen(buffer));
	
	return 0;
}

