/*
 * ProductorPersonas.cpp
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

#include "./IPC/MessageQueue/IntercambioMessageQueue.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Productor Personas: Error: Cantidad de parametros invalida\n Uso:\n ./productor [CANT PERSONAS]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int cantPersonas = 0;
	sscanf(argv[1] , "%d", &cantPersonas);
		
	sprintf(buffer, "Productor Personas: Cantidad; %d\n", cantPersonas);
	write(fileno(stdout), buffer, strlen(buffer));
		
	
	IntercambioMessageQueue intercambioMessageQueue = IntercambioMessageQueue();
	try {
		intercambioMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_PERSONAS);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Productor Personas Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
		
	// Creando personas
	for (int i = 1; i <= cantPersonas; ++i) {
		
		MensajePersona mensaje;
		mensaje.mtype = TIPO_MENSAJE;
		mensaje.idPersona = i;
		
		try {
			intercambioMessageQueue.enviarMensajePersona(mensaje);
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Productor Personas Error: %s\n", ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
		
		srand(time(NULL));
		int delay = (rand() % 3) + 1;
		sprintf(buffer, "Productor Personas: Durmiendo durante %d\n", delay);
		write(fileno(stdout), buffer, strlen(buffer));
		sleep(delay);
	}

	sprintf(buffer, "Productor Personas: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
