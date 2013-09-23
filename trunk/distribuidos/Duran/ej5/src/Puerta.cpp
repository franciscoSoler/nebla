/*
 * Puerta.cpp
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
#include "./IPC/SharedMemory/InfoSalaSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

bool estaLlena(InfoSala infoSala, int &lugarLibre) {
	lugarLibre = -1;
	for (int i = 0; i < TAM_SALA; ++i) {
		if (infoSala.idPersonas[i] == 0) {
			lugarLibre = i;
			return false;
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 1) {
		strcpy (buffer, "Puerta: Error: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	sprintf(buffer, "Puerta: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControlSala = Semaphore();	
	Semaphore semPuerta = Semaphore();
	IntercambioMessageQueue intercambioMessageQueue = IntercambioMessageQueue();
	InfoSalaSharedMemory shMemSala = InfoSalaSharedMemory();
	
	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida Sala
		semControlSala.getSemaphore((char *)DIRECTORY, ID_SEM_CONTROL_SALA, 1);
		// Obteniendo semaforo de bloqueo de la puerta
		semPuerta.getSemaphore((char *)DIRECTORY, ID_SEM_PUERTA, 1);
		// Obteniendo la cola de mensajes de intercambio
		intercambioMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_PERSONAS);
		// Obteniendo memoria compartida Sala
		shMemSala.getSharedMemory((char *)DIRECTORY, ID_SHMEM_SALA);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Puerta Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	bool deboSeguir = true;
	
	while (deboSeguir) {
		
		try {
			// Recibo una persona de la cola
			MensajePersona *mensaje;
			intercambioMessageQueue.recibirMensajePersona(TIPO_MENSAJE, mensaje);
			
			sprintf (buffer, "Puerta: Recibio a la persona %d.\n", (*mensaje).idPersona);
			write (fileno(stdout),buffer, strlen(buffer));
			
			semControlSala.wait(); 
			{
				InfoSala *infoSala = shMemSala.readInfo();

				// Verifico si hay lugar en la sala para que entre una persona				
				int lugar;
				bool salaLlena = estaLlena(*infoSala, lugar);
				while(salaLlena) {
					// No hay lugar en la sala, me bloqueo esperando que se haga un lugar
					sprintf (buffer, "Puerta: NO hay lugar en la sala, me bloqueo esperando que se vaya alguien.\n");
					write (fileno(stdout),buffer, strlen(buffer));
					(*infoSala).puertaBloqueada = true;
					shMemSala.writeInfo(infoSala);
					semControlSala.signal();
					semPuerta.wait();
					semControlSala.wait();
					{
						infoSala = shMemSala.readInfo();
						salaLlena = estaLlena((*infoSala), lugar);
					}
				}

				sprintf (buffer, "Puerta: Hay lugar en la sala en la posicion: %d.\n", lugar);
				write (fileno(stdout), buffer, strlen(buffer));

				// Verifico si hay un bus en la sala para que entre una persona		
				bool busEnSala = (*infoSala).busEnSala;
				while (! busEnSala) {
					sprintf (buffer, "Puerta: NO hay un bus en la sala, me bloqueo esperando uno.\n");
					write (fileno(stdout),buffer, strlen(buffer));
					// No hay un bus en la sala, me bloqueo esperando la llegada de alguno
					(*infoSala).puertaBloqueada = true;
					shMemSala.writeInfo(infoSala);
					semControlSala.signal();
					semPuerta.wait();
					semControlSala.wait();
					{
						infoSala = shMemSala.readInfo();
						busEnSala = (*infoSala).busEnSala;
					}
				}
				
				sprintf (buffer, "Puerta: Hay un bus en la sala. Entra la persona %d al lugar %d.\n", (*mensaje).idPersona, lugar);
				write (fileno(stdout),buffer, strlen(buffer));

				(*infoSala).puertaBloqueada = false;
				(*infoSala).idPersonas[lugar] = (*mensaje).idPersona;
				shMemSala.writeInfo(infoSala);
			}
			semControlSala.signal();	
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Puerta Error: %s\n", ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}

	sprintf(buffer, "Puerta: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
