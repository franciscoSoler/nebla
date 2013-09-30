/*
 * PuertaSalida.cpp
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

#include "./IPC/SharedMemory/InfoMuseoSharedMemory.h"
#include "./IPC/MessageQueue/SalidaMessageQueue.h"
#include "./IPC/Semaphore/Semaphore.h"

#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		strcpy (buffer, "Error Puerta Salida: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroPuerta = 0;
	sscanf(argv[1] , "%d", &nroPuerta);

	int totalPuertas = 0;
	sscanf(argv[2] , "%d", &totalPuertas);

	sprintf(buffer, "Puerta Salida: %d Iniciando\n", nroPuerta);
	write(fileno(stdout), buffer, strlen(buffer));
	
	// Obteniendo semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	if (semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1) != 0) {
		sprintf (buffer, "Puerta Salida: %d Fallo al obtener el semafoto de control\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	// Obteniendo semaforo de control de acceso a las puertas
	Semaphore semPuertas = Semaphore();	
	if (semPuertas.getSemaphore((char *)DIRECTORY, ID_SEMPUERTAS, totalPuertas) != 0) {
		sprintf (buffer, "Puerta Salida: %d Fallo al obtener el semafoto de las puertas\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Obteniendo memoria compartida
	InfoMuseoSharedMemory shMem = InfoMuseoSharedMemory();
	if (shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM) != 0) {
		sprintf (buffer, "Puerta Salida: %d Fallo al obtener la memoria\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	// Obteniendo la cola de mensajes
	SalidaMessageQueue salidaMessageQueue = SalidaMessageQueue();
	if (salidaMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_SALIDA) != 0) {
		sprintf (buffer, "Puerta Salida: %d Fallo al obtener la cola de salida\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	bool deboSeguir = true;
	
	while (deboSeguir) {
		
		MensajeSalida salida;
		if (salidaMessageQueue.recibirMensajeSalida(nroPuerta, &salida) < 0) {
			sprintf (buffer, "Puerta Salida: %d Error al recibir un mensaje de salida\n", nroPuerta);
			write (fileno(stderr),buffer, strlen(buffer));			
			exit(-1);
		}

		sprintf (buffer, "Puerta Salida: %d La persona %d se quiere ir\n", nroPuerta, salida.idPersona);
		write (fileno(stdout),buffer, strlen(buffer));
		
		semControl.wait();
		{
			InfoMuseo *info =  shMem.readInfo();
			if ((*info).cantidad == (*info).cantidadMaxima) {
				sprintf (buffer, "Puerta Salida: %d El museo estaba lleno y se fue una persona\n", nroPuerta);
				write (fileno(stdout),buffer, strlen(buffer));				
				
				for (int i = 0; i < totalPuertas; ++i) {
					// Libero a aquellas puertas de entrada que se encuentran bloqueadas
					if ((*info).puertasBloqueadas[i]) {
						semPuertas.signal(i);
					}
				}
			}
			
			(*info).cantidad--;
			deboSeguir = (((*info).abierto) || ((*info).cantidad == 0));
			shMem.writeInfo(info);
		}
		semControl.signal();
	}
	
	sprintf (buffer, "Puerta Salida: %d Terminando\n", nroPuerta);
	write (fileno(stdout),buffer, strlen(buffer));	
	
	return 0;
}
