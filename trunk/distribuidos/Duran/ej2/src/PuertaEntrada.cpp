/*
 * PuertaEntrada.cpp
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

#include "./IPC/MessageQueue/EntradaMessageQueue.h"
#include "./IPC/MessageQueue/RespuestaMessageQueue.h"

#include "./IPC/SharedMemory/InfoMuseoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"

#include "common.h"

int main(int argc, char **argv)
{
		char buffer[TAM_BUFFER];

	if (argc != 3) {
		strcpy (buffer, "Error Puerta Entrada: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroPuerta = 0;
	sscanf(argv[1] , "%d", &nroPuerta);

	int totalPuertas = 0;
	sscanf(argv[2] , "%d", &totalPuertas);

	sprintf(buffer, "Puerta Entrada: %d Iniciando\n", nroPuerta);
	write(fileno(stdout), buffer, strlen(buffer));
	
	// Obteniendo semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	if (semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1) != 0) {
		sprintf (buffer, "Puerta Entrada: %d Fallo al obtener el semafoto de control\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	// Obteniendo semaforo de control de acceso a las puertas
	Semaphore semPuertas = Semaphore();	
	if (semPuertas.getSemaphore((char *)DIRECTORY, ID_SEMPUERTAS, totalPuertas) != 0) {
		sprintf (buffer, "Puerta Entrada: %d Fallo al obtener el semafoto de las puertas\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Obteniendo memoria compartida
	InfoMuseoSharedMemory shMem = InfoMuseoSharedMemory();
	if (shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM) != 0) {
		sprintf (buffer, "Puerta Entrada: %d Fallo al obtener la memoria\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	// Obteniendo la cola de mensajes de entrada
	EntradaMessageQueue entradaMessageQueue = EntradaMessageQueue();
	if (entradaMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_ENTRADA) != 0) {
		sprintf (buffer, "Puerta Entrada: %d Fallo al obtener la cola de entrada\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	// Obteniendo la cola de mensajes de respuesta
	RespuestaMessageQueue respuestaMessageQueue = RespuestaMessageQueue();
	if (respuestaMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_RESPUESTA) != 0) {
		sprintf (buffer, "Puerta Entrada: %d Fallo al obtener la cola de respuesta\n", nroPuerta);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	bool deboSeguir = true;
	
	while (deboSeguir) {
		
		MensajeEntrada entrada;
		entradaMessageQueue.recibirMensajeEntrada(nroPuerta, &entrada);
		
		sprintf (buffer, "Puerta Entrada: %d Recibio a la persona %d\n", nroPuerta, entrada.idPersona);
		write (fileno(stdout),buffer, strlen(buffer));
		
		int semaphoreOperation = semControl.wait(0);
		if (semaphoreOperation == -1) exit(-1);
		{
			InfoMuseo *info =  shMem.readInfo();
			
			if (! (*info).abierto) {
				sprintf (buffer, "Puerta Entrada: %d Museo cerrado, la persona %d debe irse\n", nroPuerta, entrada.idPersona);
				write (fileno(stdout),buffer, strlen(buffer));
				MensajeRespuesta respuesta;
				respuesta.mtype = entrada.idPersona;
				respuesta.resultado = false;
				respuestaMessageQueue.enviarMensajeRespuesta(respuesta);
				
				deboSeguir = false;
			}
			else {
				sprintf (buffer, "Puerta Entrada: %d Museo abierto, la persona %d puede entrar\n", nroPuerta, entrada.idPersona);
				write (fileno(stdout),buffer, strlen(buffer));
				
				bool capacidadDisponible = !((*info).cantidad == (*info).cantidadMaxima);
				while ( !capacidadDisponible ) {
					
					(*info).puertasBloqueadas[nroPuerta - 1] = true;
					shMem.writeInfo(info);
					semaphoreOperation = semControl.signal(0);
					if (semaphoreOperation == -1) exit(-1);
							
					semaphoreOperation = semPuertas.wait(nroPuerta-1);
					if (semaphoreOperation == -1) exit(-1);
										
					semaphoreOperation = semControl.wait(0);
					if (semaphoreOperation == -1) exit(-1);
					{
						capacidadDisponible = !((*info).cantidad == (*info).cantidadMaxima);
					}
				}
				
				(*info).puertasBloqueadas[nroPuerta - 1] = false;
				
				sprintf (buffer, "Puerta Entrada: %d Museo abierto, capacidad disponible\n", nroPuerta);
				write (fileno(stdout),buffer, strlen(buffer));

				MensajeRespuesta respuesta;
				respuesta.mtype = entrada.idPersona;
				respuesta.resultado = true;
				respuestaMessageQueue.enviarMensajeRespuesta(respuesta);
				
				(*info).cantidad++;
				shMem.writeInfo(info);
			}			
		}
		semaphoreOperation = semControl.signal(0);
		if (semaphoreOperation == -1) exit(-1);
	}
	
	sprintf (buffer, "Puerta Entrada: %d Terminando\n", nroPuerta);
	write (fileno(stdout),buffer, strlen(buffer));
	
	return 0;
}
