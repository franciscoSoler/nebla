/* 
 * File:   Robot5.cpp
 * Author: nebla
 *
 * Created on October 22, 2013, 2:39 PM
 */

#include <cstdlib>
#include <iostream>
#include <exception>

#include "API/Robot5/ControladorRobot5.h"

#define MAX_DEMORA 10 //Demora maxima que tarda el robot 5 en ir a buscar un canasto
                       
Canasto resolverPedidoCanasto(ControladorRobot5 &controladorRobot5, PedidoCanastoAGV pedido) {
    // El robot va a buscar el canasto al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot 5: Buscando durante %d segundos\n", buscando);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(buscando * 1000 * 1000);    
    Canasto canasto = controladorRobot5.obtenerCanasto(pedido.tipoPieza);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot 5: Volviendo durante %d segundos\n", volviendo);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(volviendo * 1000 * 1000);
    
    return canasto;
}

Gabinete resolverPedidoGabinete(ControladorRobot5 &controladorRobot5, TipoProducto tipoPorudcto) {
    // El robot va a buscar el gabinete al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot 5: Buscando durante %d segundos\n", buscando);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(buscando * 1000 * 1000);    
    
    Gabinete gabinete = controladorRobot5.obtenerGabinete(tipoPorudcto);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Robot 5: Volviendo durante %d segundos\n", volviendo);
    write(fileno(stdout), buffer, strlen(buffer));
    usleep(volviendo * 1000 * 1000);
    
    return gabinete;
}

int main(int argc, char **argv) {
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Iniciando Robot 5.\n");
    write(fileno(stdout), buffer, strlen(buffer));

    ControladorRobot5 controladorRobot5;
    try {
        controladorRobot5.iniciarControlador();
    }
    catch (std::exception ex) {
        sprintf(buffer, "Robot 5: Error al iniciar el controlador: %s\n", ex.what());
        write(fileno(stderr), buffer, strlen(buffer));
        exit(-1);
    }
    
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
            sprintf(buffer, "Robot 5: Espero por un pedido.\n");
            write(fileno(stderr), buffer, strlen(buffer));
            PedidoRobot5 pedido = controladorRobot5.obtenerPedido();
                        
            switch (pedido.tipo) {
                case PEDIDO_PRODUCCION:
                    sprintf(buffer, "Robot 5: Recibi un pedido de produccion.\n");
                    write(fileno(stderr), buffer, strlen(buffer));
                    sprintf(buffer, "\tOrden de compra: %d Producto: %d Cantidad: %d Diferencia minima: %d.\n",pedido.pedidoProduccion.nroOrdenCompra, pedido.pedidoProduccion.tipo,pedido.pedidoProduccion.cantidad, pedido.pedidoProduccion.diferenciaMinima);
                    write(fileno(stderr), buffer, strlen(buffer));
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
                    sprintf(buffer, "Robot 5: Recibi un pedido de GABINETE.\n");
                    write(fileno(stderr), buffer, strlen(buffer));
                    if (productoAProducir.cantidad > 0) {
                        Gabinete gabinete = resolverPedidoGabinete(controladorRobot5, productoAProducir.tipo);
                        int ordenDeCompra = (productoAProducir.cantidad <= productoAProducir.cantidad) ? 0 : productoAProducir.nroOrdenCompra;
                        productoAProducir.cantidad--;
                        bool ultimo = (productoAProducir.cantidad == 0);
                        
                        sprintf(buffer, "Robot 5: Enviando el gabinete de tipo %d para la orden %d.\n", gabinete.tipoGabinete, ordenDeCompra);
                        write(fileno(stderr), buffer, strlen(buffer));

                        controladorRobot5.resolverPedido(gabinete, ultimo, ordenDeCompra);
                    } else {
                        sprintf(buffer, "Robot 5: ERROR: Recibi un pedido de Gabinete cuando ya se deberían haber fabricado todos lo de la orden de producción.\n");
                        write(fileno(stderr), buffer, strlen(buffer));
                        /* ERROR: Se recibio un pedido para un gabinete, cuando
                         * ya se deberían haber fabricado todos lo de la orden
                         * de producción.
                         */
                    }
                    break;
                case PEDIDO_CANASTO:
                    sprintf(buffer, "Robot 5: Recibi un pedido de CANASTO.\n");
                    write(fileno(stderr), buffer, strlen(buffer));
                    
                    Canasto canasto;
                    canasto = resolverPedidoCanasto(controladorRobot5, pedido.pedidoCanastoAgv);
                    
                    sprintf(buffer, "Robot 5: Enviando un CANASTO con %d piezas al AGV %d.\n", canasto.cantidadPiezas, pedido.pedidoCanastoAgv.idAgv);
                    write(fileno(stderr), buffer, strlen(buffer));
                    controladorRobot5.resolverPedido(canasto, pedido.pedidoCanastoAgv.idAgv);
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
