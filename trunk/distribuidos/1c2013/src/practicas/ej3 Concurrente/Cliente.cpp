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
#include <time.h>
#include <stdlib.h>

#include "./IPC/MessageQueue/ComprarMessageQueue.h"
#include "./IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "./IPC/MessageQueue/VenderMessageQueue.h"

#include "ControladorCliente.h"

#define TAM_BUFFER 255

int main(int argc, char **argv)
{
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Error Cliente: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}

	int nroCliente = 0;
	sscanf(argv[1] , "%d", &nroCliente);
	
	sprintf(buffer, "Cliente: %d Iniciando\n", nroCliente);
	write(fileno(stdout), buffer, strlen(buffer));

/*	// Obteniendo las colas de mensajes
	GetVendedorMessageQueue	getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR);
	
	ComprarMessageQueue comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR);

	VenderMessageQueue venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER);*/
	
	ControladorCliente controlador = ControladorCliente ();
	
	int nroVendedor = controlador.obtenerVendedor(nroCliente);
	sprintf(buffer, "Cliente: %d Me toco el vendedor numero: %d\n", nroCliente, nroVendedor);	
	write (fileno(stdout), buffer, strlen(buffer));
	
	/*HandShakeMessage handShake;
	handShake.mtype = 1;
	handShake.id = nroCliente;
	
	getVendedorQueue.enviarPedido(handShake);
	
	venderQueue.recibirNroVendedor (nroCliente,&handShake);
	int nroVendedor = handShake.id;
	sprintf(buffer, "Cliente: %d Me toco el vendedor numero: %d\n", nroCliente, nroVendedor);	
	write (fileno(stdout), buffer, strlen(buffer));*/
	
	bool mustContinue = true;
	srand(time(NULL));
	int cantidad = (rand()) % 4 + 1;
	sprintf(buffer, "Cliente: %d Desea comprar: %d tickets\n", nroCliente, cantidad);	
	write (fileno(stdout), buffer, strlen(buffer));
	
	
	/*RequestTicketsMessage pedido;
	pedido.mtype = nroVendedor;*/
	
	while ((cantidad > 0) && mustContinue) {
		
		int dinero = nroCliente;
		ResultadoCompra compra = controlador.comprarTicket(nroCliente, nroVendedor, dinero);
		
		/*pedido.cantTickets = 1;
		pedido.dinero = nroCliente;
	
		comprarQueue.enviarPedido(pedido);
	
		ResponseTicketsMessage resultado;
		venderQueue.recibirResultado(nroCliente, &resultado);*/
	
		if (compra.resultado) {
			// La venta fue exitosa
			sprintf(buffer, "Cliente: %d obtuvo el ticket: %s", nroCliente, compra.mensaje);
			write(fileno(stdout), buffer, strlen(buffer));
			sprintf(buffer, "Cliente: %d obtuvo vuelto: %d\n", nroCliente, compra.vuelto);
			write(fileno(stdout), buffer, strlen(buffer));
			cantidad--;
		}
		else {
			// La cantidad o el monto enviado son invalidos
			sprintf(buffer, "Cliente: %d con error: %s", nroCliente, compra.mensaje);
			write(fileno(stdout), buffer, strlen(buffer));
			mustContinue = false;
		}
	}
	
	
	controlador.finalizarCompra(nroVendedor);
	
	/*// Le envio un mensaje al vendedor para indicarle que no compro mas tickets
	pedido.cantTickets = 0;
	pedido.dinero = 0;
	comprarQueue.enviarPedido(pedido);*/
	
	sprintf(buffer, "Cliente: %d Terminado \n",nroCliente);
	write(fileno(stdout), buffer, strlen(buffer));
	
	return 0;
}
