/*
 * Robot5_AGV.cpp
 * 
 * Copyright 2013 Knoppix User <knoppix@Microknoppix>
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
#include <unistd.h>
#include <stdlib.h>

#include "Controladores/ControladorRobot5Agv.h"
#include "common.h"

#define MAX_DEMORA 15 //Demora maxima que tarda el robot 5 en ir a buscar un canasto

Canasto resolverPedido(PedidoCanasto pedido) {
    // El robot va a buscar el canasto al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot5-AGV: Buscando durante %d segundos\n", buscando);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(buscando * 1000 * 1000);
    Canasto canasto;
    canasto.tipoPieza = pedido.tipo;
    canasto.cantidadPiezas = rand() % MAX_DEMORA + 1;
    return canasto;
}

int main(int argc, char **argv) {
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Iniciando robot 5 - AGV\n");
    write(fileno(stdout), buffer, strlen(buffer));

    ControladorRobot5Agv controlador;
    bool deboSeguir = true;

    while (deboSeguir) {
        try {
                PedidoCanasto pedido = controlador.obtenerPedido();
                Canasto cansto = resolverPedido(pedido);
                controlador.resolverPedido(cansto, pedido.idAgv);
        }
        catch (std::exception ex) {
            sprintf (buffer, "Robot 5 - AGV:Error: %s\n", ex.what());
            write (fileno(stderr),buffer, strlen(buffer));
            deboSeguir = false;
	}
    }
    
    sprintf (buffer, "Robot 5 - AGV: Finalizando\n");
    write (fileno(stderr),buffer, strlen(buffer));

    return 0;
}