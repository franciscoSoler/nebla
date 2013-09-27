/*
 * Productor.cpp
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
 
#include "./IPC/SharedMemory/MuestrasSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

bool puedoCambiar(Muestras *estado, int nroMuestra) {
	for (int i = 0; i < CANT_ANALIZADORES; ++i) {
		if (! (*estado).estadoMuestras[i][nroMuestra]) {
			return false;
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Productor: Error: Cantidad de parametros invalida\n Uso:\n ./productor [NRO PRODUCTOR]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	int nroProductor = 0;
	sscanf(argv[1] , "%d", &nroProductor);
	
	sprintf(buffer, "Productor %d: Iniciando\n", nroProductor);
	write(fileno(stdout), buffer, strlen(buffer));

	Semaphore semControl = Semaphore();	
	Semaphore semProductores = Semaphore();
	Semaphore semAnalizadores = Semaphore();
		
	MuestrasSharedMemory shMem = MuestrasSharedMemory();

	try {
		// Obteniendo semaforo de control de acceso a la memoria compartida
		semControl.getSemaphore((char *)DIRECTORY, ID_SEM_CONTROL, 1);
	
		// Obteniendo semaforo de bloqueo de los procesos
		semProductores.getSemaphore((char *)DIRECTORY, ID_SEM_PRODUCTORES, CANT_MUESTRAS);
	
		// Obteniendo semaforo de bloqueo de los procesos
		semAnalizadores.getSemaphore((char *)DIRECTORY, ID_SEM_ANALIZADORES, CANT_ANALIZADORES);
	
		// Obteniendo memoria compartida
		shMem.getSharedMemory((char *)DIRECTORY, ID_SHMEM_MUESTRAS);
	}
	catch (IPCException &ex) {
		sprintf (buffer, "Productor %d: Error Init: %s\n", nroProductor, ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	bool deboSeguir = true;

	while (deboSeguir) {

		try {
			semControl.wait();

			Muestras *estado = shMem.readInfo();

			// Verifico si la muestra ya fue analizada por todos los analizadores
			bool cambiar = puedoCambiar(estado, nroProductor);

			while (! cambiar) {	
				sprintf(buffer, "Productor %d: La muestra todavia no fue analizada por completo.\n",nroProductor);
				write(fileno(stdout), buffer, strlen(buffer));

				// La muestra no fue analizada por completo, me bloqueo esperando que todos los analizadores terminen
				(*estado).productorBloqueado[nroProductor] = true;
				shMem.writeInfo(estado);
				// Libero la memoria compartida
				semControl.signal();
				// Me bloqueo en mi semaforo
				semProductores.wait(nroProductor);
				// Obtengo la memoria compartida y vuelvo a verificar si debo producir
				semControl.wait();
				estado = shMem.readInfo();
				cambiar = puedoCambiar(estado, nroProductor);
			}

			// Todos los analizadores analizaron la muestra, puedo colocar una nueva muestra
			sprintf(buffer, "Productor %d: La muestra ya fue analizada por completo, procedo a cambiarla.\n",nroProductor);
			write(fileno(stdout), buffer, strlen(buffer));

			// Libero la memoria compartida mientras busco una nueva muestra
			semControl.signal();

			// Busco una nueva muestra
			srand(time(NULL));
			int delay = (rand() % 3) + 1;
			sprintf(buffer, "Productor %d: Buscando una nueva muestra durante %d\n", nroProductor, delay);
			write(fileno(stdout), buffer, strlen(buffer));
			sleep(delay);

			// Una vez depositada la nueva muestra, desbloqueo a los analizadores necesarios
			semControl.wait();
			estado = shMem.readInfo();

			//  Indico que hay una nueva muestra para analizar y
			//  verifico si hay algun analizador bloqueado en mi muestra y lo libero
			for (int j = 0; j < CANT_ANALIZADORES; ++j) {
				(*estado).estadoMuestras[j][nroProductor] = false;

				if ((*estado).analizadorBloqueado[j] == nroProductor) {
					sprintf(buffer, "Productor %d: Libero al analizador %d\n", nroProductor, j);
					write(fileno(stdout), buffer, strlen(buffer));

					// Libero al analizador que esta esperando la muestra
					(*estado).analizadorBloqueado[j] = CANT_MUESTRAS;
					semAnalizadores.signal(j);
				}
			}

			shMem.writeInfo(estado);

			semControl.signal();

		}
		catch (IPCException &ex) {
			sprintf (buffer, "Productor %d: Error: %s\n", nroProductor, ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}

	sprintf(buffer, "Productor %d: Terminado \n",nroProductor);
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
