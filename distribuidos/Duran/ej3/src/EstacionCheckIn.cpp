/*
 * EstacionCheckIn.cpp
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

#include "./IPC/IPCException.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/SharedMemory/InfoVuelosSharedMemory.h"
#include "./IPC/MessageQueue/IntercambioMessageQueue.h"
#include "./IPC/MessageQueue/TransportadoraMessageQueue.h"

#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Estacion CheckIn: Cantidad de parametros invalida.\n\tUso: ./estacionCheckIn [NRO VUELO]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroVuelo = 0;
	sscanf(argv[1] , "%d", &nroVuelo);

	sprintf(buffer, "Estacion CheckIn: %d Iniciando\n", nroVuelo);
	write(fileno(stdout), buffer, strlen(buffer));
	
	Semaphore semControl = Semaphore();	
	InfoVuelosSharedMemory shMem = InfoVuelosSharedMemory();
	IntercambioMessageQueue intercambioMessageQueue = IntercambioMessageQueue();
	TransportadoraMessageQueue transportadoraMessageQueue = TransportadoraMessageQueue();
	
	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida
		semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);

		// Obteniendo memoria compartida
		shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM);

		// Obteniendo la cola de intercambio
		intercambioMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_INTERCAMBIO);
		
		// Obteniendo la cola de transporte
		transportadoraMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_TRANSPORTADORA);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Estacion CheckIn %d: %s\n", nroVuelo, ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
		
	bool deboSeguir = true;
	
	while (deboSeguir) {
		try {
			MensajeIntercambioEquipaje intercambioEquipaje;
			intercambioMessageQueue.recibirMensajeConEquipaje(nroVuelo, &intercambioEquipaje);

			sprintf (buffer, "Estacion CheckIn: %d Recibio un equipaje de %d\n", nroVuelo, intercambioEquipaje.nroPersona);
			write (fileno(stdout),buffer, strlen(buffer));
		
			semControl.wait();
			{
				Informacion info =  *(shMem.readInfo());
				InfoVuelo vuelo = info.vuelos[nroVuelo-1];
				vuelo.cantidad++;
				deboSeguir = !vuelo.listo;
				info.vuelos[nroVuelo-1] = vuelo;
				shMem.writeInfo(&info);
			}
			semControl.signal();
		
			MensajeVerificacionEquipaje mensajeVerificacion;
			mensajeVerificacion.mtype = TIPO_VERIFICACION;
			mensajeVerificacion.mensajeEquipajeVerificado = intercambioEquipaje;
			transportadoraMessageQueue.enviarMensajeAVerificar(mensajeVerificacion);
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Estacion CheckIn %d: %s\n", nroVuelo, ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf (buffer, "Estacion CheckIn: %d Terminando\n", nroVuelo);
	write (fileno(stdout),buffer, strlen(buffer));	
	
	return 0;
}
