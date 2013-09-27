/*
 * EstacionSospechosos.cpp
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

#include "./IPC/MessageQueue/PeligrososMessageQueue.h"
#include "./IPC/MessageQueue/VerificadosMessageQueue.h"
#include "./IPC/MessageQueue/TransportadoraMessageQueue.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Estacion Sospechosos: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int cantVuelos = 0;
	sscanf(argv[1] , "%d", &cantVuelos);

	sprintf(buffer, "Estacion Sospechosos: Iniciando con vuelos: %d\n", cantVuelos);
	write (fileno(stdout),buffer, strlen(buffer));

	PeligrososMessageQueue peligrososMessageQueue = PeligrososMessageQueue();
	try {
		peligrososMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_PELIGROSOS);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Estacion Sospechosos: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	bool deboSeguir = true;
	
	int vuelos[TAM_BUFFER];
	for (int i = 0; i < TAM_BUFFER; ++i) {
		vuelos[i] = 0;
	}
	
	while (deboSeguir) {
		try {
			MensajeEquipajeSospechoso mensajeSospechoso;
			peligrososMessageQueue.recibirMensajeConEquipajePeligroso(TIPO_SOSPECHOSOS, &mensajeSospechoso);
					
			srand(time(NULL));
			int verificacion = rand() % 10 + 1;
			sprintf (buffer, "Estacion Sospechosos: Verificando durante: %d\n", verificacion);
			write (fileno(stdout),buffer, strlen(buffer));
			usleep(verificacion*1000*1000);
			
			/*
			 * Listar para cada vuelo la cantidad de equipajes sospechoso
			 */
			 vuelos[mensajeSospechoso.vueloDestino-1]++;
		}
		catch (IPCException &ex) {

			sprintf (buffer, "Cantidad Equipajes Sospechosos\n");
			write (fileno(stdout),buffer, strlen(buffer));		
			for (int i = 0; i < cantVuelos; ++i) {
				sprintf (buffer, "\tVuelo: %d Cantidad: %d\n", i+1, vuelos[i]);
				write (fileno(stdout),buffer, strlen(buffer));				
			}
			
			sprintf (buffer, "Estacion Sospechosos: %s\n", ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf (buffer, "Estacion Sospechosos: Terminando\n");
	write (fileno(stdout),buffer, strlen(buffer));	
	
	return 0;
}
