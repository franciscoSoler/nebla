/*
 * Bus.cpp
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
#include "./IPC/IPCException.h"
#include "common.h"

bool estaVacia(InfoSala infoSala) {
	for (int i = 0; i < TAM_SALA; ++i) {
		if (infoSala.idPersonas[i] != 0) {
			return false;
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		strcpy (buffer, "Bus: Error: Cantidad de parametros invalida.\n\tUso:\n\t./bus [NRO BUS] [CANT BUSES]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	int nroBus = 0;
	sscanf(argv[1] , "%d", &nroBus);
	
	int cantBuses = 0;
	sscanf(argv[2] , "%d", &cantBuses);
	
	sprintf(buffer, "Bus: %d Iniciando\n",nroBus);
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControlSala = Semaphore();	
	Semaphore semPuerta = Semaphore();
	Semaphore semBuses = Semaphore();
	
	InfoSalaSharedMemory shMemSala = InfoSalaSharedMemory();
	
	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida Sala
		semControlSala.getSemaphore((char *)DIRECTORY, ID_SEM_CONTROL_SALA, 1);
		// Obteniendo semaforo de bloqueo de la puerta
		semPuerta.getSemaphore((char *)DIRECTORY, ID_SEM_PUERTA, 1);
		// Obteniendo semaforo de bloqueo de los buses
		semBuses.getSemaphore((char *)DIRECTORY, ID_SEM_BUSES, cantBuses);
		// Obteniendo memoria compartida Sala
		shMemSala.getSharedMemory((char *)DIRECTORY, ID_SHMEM_SALA);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Bus Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	bool deboSeguir = true;
	
	while (deboSeguir) {
		
		try {
			
			semControlSala.wait(); 
			{
				InfoSala *infoSala = shMemSala.readInfo();

				// Verifico si hay algun bus ya en la sala
				bool busEnSala = (*infoSala).busEnSala;
				while (busEnSala) {
					sprintf (buffer, "Bus %d: Ya hay un bus en la sala, me bloqueo esperando que se vaya.\n", nroBus);
					write (fileno(stdout),buffer, strlen(buffer));
					// Ya hay otro bus en la sala, me bloqueo esperando que se libere
					(*infoSala).busBloqueado[nroBus] = true;
					shMemSala.writeInfo(infoSala);
					semControlSala.signal();
					semBuses.wait(nroBus);
					
					sprintf (buffer, "Bus %d: Me desbloqueo, verifico si entro otro bus en la sala.\n", nroBus);
					write (fileno(stdout),buffer, strlen(buffer));
					
					semControlSala.wait();
					{
						infoSala = shMemSala.readInfo();
						busEnSala = (*infoSala).busEnSala;
					}
				}
				
				(*infoSala).busEnSala = true;
				sprintf (buffer, "Bus %d: Entro en la sala.\n", nroBus);
				write (fileno(stdout),buffer, strlen(buffer));

				// Si la puerta esta bloqueada, le indico que se libere
				if ((*infoSala).puertaBloqueada) {
					semPuerta.signal();
				}
				
				int cantidad = 0;
				bool vacia = estaVacia(*infoSala);
				
				if (vacia) {
					// Espero un rato para ver si llega alguien
					sprintf (buffer, "Bus %d: La sala esta vacia, espero para ver si llega alguien.\n", nroBus);
					write (fileno(stdout),buffer, strlen(buffer));
					
					shMemSala.writeInfo(infoSala);
					semControlSala.signal();
					
					srand(time(NULL));
					int delay = (rand() % 10) + 1;
					sleep(delay);
					
					semControlSala.wait();
					{
						infoSala = shMemSala.readInfo();
					}
				}
				
				vacia = estaVacia(*infoSala);
				int i = 0;				
				while ((cantidad < CAPACIDAD_BUS) && (! vacia)) {
					// Comienzo a entrar personas
					int idPersona = 0;
					while (idPersona == 0) {
						int posicion = (i++)%TAM_SALA;
						idPersona = (*infoSala).idPersonas[posicion];
						(*infoSala).idPersonas[posicion] = 0;
					}
					
					sprintf (buffer, "Bus %d: Subiendo a la persona %d.\n", nroBus, idPersona);
					write (fileno(stdout),buffer, strlen(buffer));
					cantidad++;
			
					// Si la puerta esta bloqueada, le indico que se libere
					if ((*infoSala).puertaBloqueada) {
						(*infoSala).puertaBloqueada = false;
						semPuerta.signal();
					}
				
					// Indico a la persona que debe subir, mientras sube la persona, libero la sala.
					shMemSala.writeInfo(infoSala);
					semControlSala.signal();
					
					srand(time(NULL));
					int delay = (rand() % 10) + 1;
					sleep(delay);
					
					semControlSala.wait();
					infoSala = shMemSala.readInfo();
					vacia = estaVacia(*infoSala);
				}
				
				sprintf (buffer, "Bus %d: Se va de la sala con %d personas.\n", nroBus, cantidad);
				write (fileno(stdout),buffer, strlen(buffer));	
			
				(*infoSala).busBloqueado[nroBus] = false;		
				// Desbloqueo a todos los buses que quieran entar en la sala
				for (int i = 0; i < cantBuses; ++i) {
					if ((*infoSala).busBloqueado[i]) {
						(*infoSala).busBloqueado[i] = false;
						
						sprintf (buffer, "Bus %d: Desbloqueando al bus %d.\n", nroBus, i);
						write (fileno(stdout),buffer, strlen(buffer));	
						
						semBuses.signal(i);
					}
				}
				
				(*infoSala).busEnSala = false;
				shMemSala.writeInfo(infoSala);
			}
			semControlSala.signal();
			
			// El bus se va a hacer su recorrido
			srand(time(NULL));
			int delay = (rand() % 20) + 1;
			sleep(20);
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Bus %d: Error: %s\n", nroBus, ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}

	sprintf(buffer, "Bus: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
