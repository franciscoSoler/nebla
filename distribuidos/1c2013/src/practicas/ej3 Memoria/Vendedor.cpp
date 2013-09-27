/*
 * Productos.cpp
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

#include "ControladorVendedor.h"
#include "ControladorTickets.h"

#include "common.h"

#define TAM_BUFFER 255

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Vendedor: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroVendedor = 0;
	sscanf(argv[1] , "%d", &nroVendedor);

	sprintf(buffer, "Vendedor: %d Iniciando\n", nroVendedor);
	write(fileno(stdout), buffer, strlen(buffer));
	
	ControladorVendedor controladorVendedor = ControladorVendedor(nroVendedor);
	ControladorTickets controladorTickets = ControladorTickets();
	controladorTickets.conectarMemoria();
		
	bool mustContinue = true;
		
	while (mustContinue) {	
		controladorVendedor.enviarVendedorLibre();
			
		TicketsInfo ticketsInfo = controladorVendedor.obtenerPedido();
		int cantidad = ticketsInfo.cantidad;
		
		while (cantidad > 0) {
				
			int dinero = ticketsInfo.dinero;
			sprintf(buffer, "Vendedor: %d Cantidad requerida: %d Dinero: %d\n", nroVendedor, cantidad, dinero);
			write (fileno(stdout), buffer, strlen(buffer));
			
			ResultadoCompra resultadoOperacion;
			controladorTickets.obtenerTickets(cantidad, dinero, resultadoOperacion);
						
			// Se envia el resultado de la operacion al cliente
			controladorVendedor.enviarResultado(resultadoOperacion);
			
			// Se pregunta por un nuevo ticket
			ticketsInfo = controladorVendedor.obtenerPedido();
			cantidad = ticketsInfo.cantidad;		
		}
		controladorVendedor.finalizarVenta();
	}
	
	controladorTickets.desconectarMemoria();

	
	sprintf(buffer, "Vendedor: %d Terminado \n", nroVendedor);
	write(fileno(stdout), buffer, strlen(buffer));
	
	return 0;
}

