/*
 * EstacionVerificacion.cpp
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
		strcpy (buffer, "Error Estacion Verificacion: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroEstacion = 0;
	sscanf(argv[1] , "%d", &nroEstacion);

	sprintf(buffer, "Estacion Verificacion: %d Iniciando\n",nroEstacion);
	write(fileno(stdout), buffer, strlen(buffer));

	TransportadoraMessageQueue transportadoraMessageQueue = TransportadoraMessageQueue();
	VerificadosMessageQueue verificadosMessageQueue = VerificadosMessageQueue();	
	PeligrososMessageQueue peligrososMessageQueue = PeligrososMessageQueue();
	try {
		// Obteniendo la cola de transporte
		transportadoraMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_TRANSPORTADORA);
		
		// Obteniendo la cola de verificados
		verificadosMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_VERIFICADOS);
		
		// Obteniendo la cola de peligrosos
		peligrososMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_PELIGROSOS);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Estacion Verificacion %d: %s\n", nroEstacion, ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	bool deboSeguir = true;
	
	int noPeligrosos = 0;
	int peligrosos = 0;
	
	while (deboSeguir) {
		try {
			MensajeVerificacionEquipaje mensajeVerificacion;
			transportadoraMessageQueue.recibirMensajeAVerificar(TIPO_VERIFICACION, &mensajeVerificacion);
					
			srand(time(NULL));
			int verificacion = rand() % 10 + 1;
			sprintf (buffer, "Estacion Verificacion: %d Verificando durante: %d\n", nroEstacion, verificacion);
			write (fileno(stdout),buffer, strlen(buffer));
			usleep(verificacion*1000*1000);
			
			if (mensajeVerificacion.mensajeEquipajeVerificado.equipaje.contenido == SUSTANCIA_NO_PELIGROSA) {
				sprintf (buffer, "Estacion Verificacion: %d El equipaje no es peligroso\n", nroEstacion, verificacion);
				write (fileno(stdout),buffer, strlen(buffer));
				noPeligrosos++;
				verificadosMessageQueue.enviarMensajeVerificado(mensajeVerificacion.mensajeEquipajeVerificado);
			}
			else {
				sprintf (buffer, "Estacion Verificacion: %d El equipaje tiene la sustancia: %d\n", nroEstacion, mensajeVerificacion.mensajeEquipajeVerificado.equipaje.contenido);
				write (fileno(stdout),buffer, strlen(buffer));
				MensajeEquipajeSospechoso mensajeSospechoso;
				mensajeSospechoso.mtype = TIPO_SOSPECHOSOS;
				mensajeSospechoso.vueloDestino = mensajeVerificacion.mensajeEquipajeVerificado.mtype;
				mensajeSospechoso.equipajeSospechoso = mensajeVerificacion.mensajeEquipajeVerificado.equipaje;
				peligrosos++;
				peligrososMessageQueue.enviarMensajeConEquipajePeligroso(mensajeSospechoso);
			}
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Estacion Verificacion %d: Recibio:\n\tNO Peligrosos:%d\n\tPeligrosos: %d\n", nroEstacion, noPeligrosos, peligrosos);
			write (fileno(stdout), buffer, strlen(buffer));			
			
			sprintf (buffer, "Estacion Verificacion %d: %s\n", nroEstacion, ex.getDescription());			
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf (buffer, "Estacion Verificacion: %d Terminando\n", nroEstacion);
	write (fileno(stdout),buffer, strlen(buffer));	
	
	return 0;
}
