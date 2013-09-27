/*
 * Terminator.cpp
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

#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/IPCException.h"
#include "common.h"

int main(int argc, char **argv)
{	
	char buffer[TAM_BUFFER];

	try {
		Semaphore semEmbotelladora = Semaphore();
		semEmbotelladora.getSemaphore((char *)DIRECTORY, ID_SEM_EMBOTELLADORA, 1);
		semEmbotelladora.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
		
	try {
		Semaphore semEtiquetadora = Semaphore();
		semEtiquetadora.getSemaphore((char *)DIRECTORY, ID_SEM_ETIQUETADORA, 1);
		semEtiquetadora.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
			
	try {
		Semaphore semTapadora = Semaphore();
		semTapadora.getSemaphore((char *)DIRECTORY, ID_SEM_TAPADORA, 1);
		semTapadora.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}
	
	try {
		Semaphore semAlmacenadora = Semaphore();
		semAlmacenadora.getSemaphore((char *)DIRECTORY, ID_SEM_ALMACENADORA, 1);
		semAlmacenadora.destroy();
	}	
	catch (IPCException &ex) {
		sprintf (buffer, "Terminator Error: %s\n", ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
	}

	return 0;
}

