/*
 * Analizador.cpp
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

#include "./IPC/SharedMemory/SamplesTableSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "common.h"

#define TAM_BUFFER 255

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Analizador: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroAnalizador = 0;
	sscanf(argv[1] , "%d", &nroAnalizador);
	
	sprintf(buffer, "Analizador: %d Iniciando\n", nroAnalizador);
	write(fileno(stdout), buffer, strlen(buffer));

	// Obteniendo los semaforos que controlan el avance de productores y analizadores
	Semaphore semProductores = Semaphore();
	semProductores.getSemaphore((char *)DIRECTORY,ID_SEMPRODUCTORES, CANT_PRODUCTORES);	
	Semaphore semAnalizadores =  Semaphore();
	semAnalizadores.getSemaphore((char *)DIRECTORY,ID_SEMANALIZADORES, CANT_ANALIZADORES);	
	
	// Obteniendo el semaforo que controla el acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	semControl.getSemaphore((char *)DIRECTORY, ID_SEMCONTROL, 1);
	
	// Obteniendo la memoria compartida
	SamplesTableSharedMemory table = SamplesTableSharedMemory();
	table.getSharedMemory((char *)DIRECTORY, ID_SHMEM);
	
	bool mustContinue = true;
	int nroMuestra = 0;
	
	while (mustContinue) {
		for (nroMuestra = 0; nroMuestra < CANT_PRODUCTORES; ++nroMuestra) {
			bool sampleReady = false;
			while (!sampleReady) {
								
				semAnalizadores.wait(nroAnalizador);
				semControl.wait(); 
				{
					SamplesTable samplesTable = *(table.readInfo());
					sampleReady = samplesTable.sampleReady[nroMuestra][nroAnalizador];
					samplesTable.analiza[nroAnalizador] = true;
					table.writeInfo(&samplesTable);
					
				}
				semControl.signal();
			}
			
			Sample sampleUnderAnalisis;
			semControl.wait(); 
			{
				SamplesTable samplesTable = *(table.readInfo());
				sampleUnderAnalisis = samplesTable.muestra[nroMuestra];
				samplesTable.analiza[nroAnalizador] = false;
				table.writeInfo(&samplesTable);
			}
			semControl.signal();
			
			// Analizing
			srand(getpid());
			int delay = (rand() % 5) + 1;
			sprintf(buffer, "Analizador: %d Analizando durante %d\n", nroAnalizador, delay);
			write(fileno(stdout), buffer, strlen(buffer));
			sleep(delay);

			semControl.wait(); 
			{
				SamplesTable samplesTable = *(table.readInfo());
				
				if (delay % sampleUnderAnalisis.valor != 0) {			
					// La muestra esta infectada
					samplesTable.muestra[nroMuestra].estado = false;
				}
				samplesTable.sampleReady[nroMuestra][nroAnalizador] = true;
				samplesTable.analiza[nroAnalizador] = false;
				
				bool sampleReady = true;
				for (int i = 0; i < CANT_ANALIZADORES; ++i) {
					sampleReady = sampleReady && samplesTable.sampleReady[nroMuestra][i];
				}
				
				if (sampleReady) {
					semProductores.signal(nroMuestra);
					if (samplesTable.muestra[nroMuestra].estado)
						sprintf(buffer, "Analizador: %d Encontro que la %d muestra es correcta\n", nroAnalizador, nroMuestra);
					else
						sprintf(buffer, "Analizador: %d Encontro que la %d muestra esta infectada\n", nroAnalizador, nroMuestra);						

					write(fileno(stdout), buffer, strlen(buffer));
				}
				
				table.writeInfo(&samplesTable);
			}
			semControl.signal();
			
		}
	}

	return 0;
}
