/*
 * Procesador.cpp
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
 
#include <time.h>
 
#include "./IPC/SharedMemory/BufferSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 3) {
		strcpy (buffer, "Procesador: Error: Cantidad de parametros invalida\n Uso:\n ./procesador [NRO PROCESADOR] [CANT PROCESADORES]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	int nroProcesador = 0;
	sscanf(argv[1] , "%d", &nroProcesador);
	
	int cantProcesadores = 0;
	sscanf(argv[2] , "%d", &cantProcesadores);
	
	sprintf(buffer, "Procesador %d: Iniciando\n", nroProcesador);
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControl = Semaphore();	
	Semaphore semProcesadores = Semaphore();
	
	BufferSharedMemory shMemBuffer = BufferSharedMemory();
	
	try {
		// Obtengo semaforo de control de acceso a la memoria compartida
		semControl.getSemaphore((char *)DIRECTORY, ID_SEM_CONTROL_BUFFER, 1);

		// Obtengo semaforo de bloqueo de los procesadores
		semProcesadores.getSemaphore((char *)DIRECTORY, ID_SEM_PROCESADORES, cantProcesadores);

		// Obtengo memoria compartida Buffer
		shMemBuffer.getSharedMemory((char *)DIRECTORY, ID_SHMEM_BUFFER);
	
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Procesador %d: Error Init: %s\n", nroProcesador, ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	int total = 0;
	bool deboSeguir = true;

	while (deboSeguir) {
		
		try {
			semProcesadores.wait(nroProcesador);
			
			// Comienzo el procesamiento
			srand(time(NULL));
			int delay = (rand() % 3) + 1;
			sprintf(buffer, "Procesador %d: Procesando elemento %d durante %d\n", nroProcesador, total, delay);
			write(fileno(stdout), buffer, strlen(buffer));
			sleep(delay);
			total++;
			
			semControl.wait();
			{
				Buffer *buffer = shMemBuffer.readInfo();
				(*buffer).informacion[nroProcesador] = total;
				(*buffer).contador++;
				
				if ((*buffer).contador == cantProcesadores) {
					// Todos terminaron de procesar, debo desbloquearlos y resetear el contador
					(*buffer).contador = 0;
					for (int i = 0; i < cantProcesadores; ++i) {
						semProcesadores.signal(i);
					}
				}
				
				shMemBuffer.writeInfo(buffer);
			}
			semControl.signal();
			
			deboSeguir = (total < TOTAL);
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Procesador %d: Error: %s\n", nroProcesador, ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf(buffer, "Procesador %d: Terminado \n",nroProcesador);
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
