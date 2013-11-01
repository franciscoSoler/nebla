/* 
 * File:   Robot5.cpp
 * Author: nebla
 *
 * Created on October 22, 2013, 2:39 PM
 */

#include <cstdlib>
#include <iostream>
#include <exception>

#include "Controladores/ControladorRobot5.h"

#define MAX_DEMORA 15 //Demora maxima que tarda el robot 5 en ir a buscar un canasto
                       
Canasto resolverPedidoCanasto(ControladorRobot5 &controladorRobot5, PedidoCanasto pedido) {
    // El robot va a buscar el canasto al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot5: Buscando durante %d segundos\n", buscando);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(buscando * 1000 * 1000);    
    Canasto canasto = controladorRobot5.obtenerCanasto(pedido.tipoPieza);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot5: Volviendo durante %d segundos\n", volviendo);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(volviendo * 1000 * 1000);
    
    return canasto;
}

Gabinete resolverPedidoGabinete(ControladorRobot5 &controladorRobot5, TipoProducto tipoPorudcto) {
    // El robot va a buscar el gabinete al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot5: Buscando durante %d segundos\n", buscando);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(buscando * 1000 * 1000);    
    
    Gabinete gabinete = controladorRobot5.obtenerGabinete(tipoPorudcto);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot5: Volviendo durante %d segundos\n", volviendo);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(volviendo * 1000 * 1000);
    
    return gabinete;
}

int main(int argc, char **argv) {
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Iniciando robot 5.\n");
    write(fileno(stdout), buffer, strlen(buffer));

    ControladorRobot5 controladorRobot5;
    //ControladorAlamacenPiezas controladorAlmacen;
    
    bool deboSeguir = true;
    
    /* El pedido de produccion consta de un tipo de producto a producir
     * y una cantidad para el mismo.
     */
    PedidoProduccion productoAProducir;
    productoAProducir.cantidad = 0;

    while (deboSeguir) {
        try {
            /* Obtengo un pedido para el robot, el mismo puede ser
             * un pedido de un canasto de un agv, un pedido por un 
             * gabinete o un pedido de producción de un producto.
             */
            PedidoRobot5 pedido = controladorRobot5.obtenerPedido();

            switch (pedido.tipo) {
                case PEDIDO_PRODUCCION:
                    /* El robot recibió una nueva orden de producción */
                    if (productoAProducir.cantidad == 0) {
                        productoAProducir = pedido.pedidoProduccion;
                    } else {
                        /* ERROR: Se recibio una orden de producción sin 
                         * antes haber terminado la anterior.
                         */
                    }
                    break;
                case PEDIDO_GABINETE:
                    if (productoAProducir.cantidad > 0) {
                        Gabinete gabinete = resolverPedidoGabinete(controladorRobot5, productoAProducir.tipo);
                        int ordenDeCompra = (productoAProducir.cantidad <= productoAProducir.cantidad) ? 0 : productoAProducir.nroOrdenCompra;
                        productoAProducir.cantidad--;
                        bool ultimo = (productoAProducir.cantidad == 0);
                        controladorRobot5.resolverPedido(gabinete, ultimo, ordenDeCompra);
                    } else {
                        /* ERROR: Se recibio un pedido para un gabinete, cuando
                         * ya se deberían haber fabricado todos lo de la orden
                         * de producción.
                         */
                    }
                    break;
                case PEDIDO_CANASTO:
                    Canasto canasto;
                    canasto = resolverPedidoCanasto(controladorRobot5, pedido.pedidoCanasto);
                    controladorRobot5.resolverPedido(canasto, pedido.pedidoCanasto.idAgv);
                    break;
                    
                default:
                    break;
            }
        }        
        catch (std::exception ex) {
            sprintf(buffer, "Robot 5: Error: %s\n", ex.what());
            write(fileno(stderr), buffer, strlen(buffer));
            deboSeguir = false;
        }
    }
    
    sprintf (buffer, "Robot 5: Finalizando\n");
    write (fileno(stderr),buffer, strlen(buffer));

    return 0;
}
