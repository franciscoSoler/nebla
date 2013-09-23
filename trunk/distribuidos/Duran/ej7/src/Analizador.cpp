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
 
#include <time.h>
 
#include "./IPC/SharedMemory/MuestrasSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Analizador: Error: Cantidad de parametros invalida\n Uso:\n ./analizador [NRO ANALIZADOR]\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	int nroAnalizador = 0;
	sscanf(argv[1] , "%d", &nroAnalizador);
	
	sprintf(buffer, "Analizador %d: Iniciando\n", nroAnalizador);
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
		sprintf (buffer, "Analizador %d: Error Init: %s\n", nroAnalizador, ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

	int muestraBajoAnalisis = 0;
	bool deboSeguir = true;

	while (deboSeguir) {
		
		try {
			semControl.wait();
						
			Muestras *estado = shMem.readInfo();
				
			// Verifico si la muestra que tengo que analizar ya esta disponible
			bool yaAnalizada = (*estado).estadoMuestras[nroAnalizador][muestraBajoAnalisis];
				
			while (yaAnalizada) {	
				sprintf(buffer, "Analizador %d: La muestra %d todavia no fue reseteada.\n",nroAnalizador, muestraBajoAnalisis);
				write(fileno(stdout), buffer, strlen(buffer));
					
				// La muestra ya fue analizada, me bloqueo esperando que el productor coloque una nueva
				(*estado).analizadorBloqueado[nroAnalizador] = muestraBajoAnalisis;
				shMem.writeInfo(estado);
				// Libero la memoria compartida
				semControl.signal();
				// Me bloqueo en mi semaforo
				semAnalizadores.wait(nroAnalizador);
				// Obtengo la memoria compartida y vuelvo a verificar si debo analizar
				semControl.wait();
				estado = shMem.readInfo();
				yaAnalizada = (*estado).estadoMuestras[nroAnalizador][muestraBajoAnalisis];
			}
				
			// La muestra ya fue reseteada, procedo a analizarla y continuar a la proxima
			sprintf(buffer, "Analizador %d: La muestra %d ya fue reseteada, procedo a analizarla.\n",nroAnalizador, muestraBajoAnalisis);
			write(fileno(stdout), buffer, strlen(buffer));				
			
			// Libero la memoria compartida mientras analizo
			semControl.signal();
				
			// Comienzo el analisis
			srand(time(NULL));
			int delay = (rand() % 3) + 1;
			sprintf(buffer, "Analizador %d: Analizando muestra %d durante %d\n", nroAnalizador, muestraBajoAnalisis, delay);
			write(fileno(stdout), buffer, strlen(buffer));
			sleep(delay);
				
			// Una vez analizada la muestra, indico que ya termine el analisis, y desbloqueo al productor de ser necesario				
			semControl.wait();
			estado = shMem.readInfo();
			
			// Indico que se analice la muestra
			(*estado).estadoMuestras[nroAnalizador][muestraBajoAnalisis] = true;
			
			// Verifico si el productor esta bloqueado
			if ((*estado).productorBloqueado[muestraBajoAnalisis]) {
				// Libero al productor
				(*estado).productorBloqueado[muestraBajoAnalisis] = false;
				semProductores.signal(muestraBajoAnalisis);
			}			
			
			shMem.writeInfo(estado);
			
			semControl.signal();
			
			// Continuo a la siguiente muestra
			muestraBajoAnalisis = (muestraBajoAnalisis + 1) % CANT_MUESTRAS;
						
		}
		catch (IPCException &ex) {
			sprintf (buffer, "Analizador %d: Error: %s\n", nroAnalizador, ex.getDescription());
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
	}
	
	sprintf(buffer, "Analizador %d: Terminado \n",nroAnalizador);
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
