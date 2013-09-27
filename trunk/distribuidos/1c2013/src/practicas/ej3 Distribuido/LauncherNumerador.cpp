/*
 * LauncherNumerador.cpp
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

#include "./IPC/SharedMemory/NumeradorSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./Numerador/commonNumerador.h"

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 1) {
		strcpy (buffer, "Launcher Numerador: Error: Cantidad de parametros invalida\n Uso:\n ./launcherNumerador\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
			
	// Creando semaforo de control de acceso a la memoria compartida
	Semaphore semControl = Semaphore();	
	
	int resultado = semControl.createSemaphore((char *)DIRECTORY_NUM, ID_SEMCONTROL_NUMERADOR, 1);
	if (resultado < 0) exit(-1);
	
	semControl.initializeSemaphore(0, 1);
	
	// Creando memoria compartida
	NumeradorSharedMemory shMem = NumeradorSharedMemory();
	resultado = shMem.createSharedMemory((char *)DIRECTORY_NUM, ID_SHMEM_NUMERADOR);
	if (resultado < 0) exit(-1);
	
	NumeradorInfo initialInfo;
	
	int i = 0;
	for (i = 0; i < MAX_CLIENTES; ++i) {
		initialInfo.numerosClientes[i] = 0;
	}
	for (i = 0; i < MAX_VENDEDORES; ++i) {
		initialInfo.numerosVendedores[i] = 0;
	}
		
	semControl.wait(0);
	{
		shMem.writeInfo(&initialInfo);
	}		
	semControl.signal(0);
			
	/*// Creando numerador
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./Numerador/numerador_server", "numerador_server", (char *) 0);
		sprintf(buffer, "Launcher Numerador: Execlp ./numerador_server error: %s\n", strerror(errno));
		write(fileno(stdout), buffer, strlen(buffer));
	}*/
	
	sprintf(buffer, "Launcher Numerador: Terminado \n");
	write(fileno(stdout), buffer, strlen(buffer));

	return 0;
}
