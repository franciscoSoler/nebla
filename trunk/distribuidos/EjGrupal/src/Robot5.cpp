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
#include "Logger.h"

#define MAX_DEMORA 10 //Demora maxima que tarda el robot 5 en ir a buscar un canasto
                       
Canasto resolverPedidoCanasto(ControladorRobot5 &controladorRobot5, PedidoCanastoAGV pedido) {
    // El robot va a buscar el canasto al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Buscando durante %d segundos", buscando);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);

    usleep(buscando * 1000 * 1000);    
    Canasto canasto = controladorRobot5.obtenerCanasto(pedido.tipoPieza);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Volviendo durante %d segundos", volviendo);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);
    usleep(volviendo * 1000 * 1000);
    
    return canasto;
}

Gabinete resolverPedidoGabinete(ControladorRobot5 &controladorRobot5, TipoProducto tipoPorudcto) {
    // El robot va a buscar el gabinete al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Buscando durante %d segundos.", buscando);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);
    usleep(buscando * 1000 * 1000);    
    
    Gabinete gabinete = controladorRobot5.obtenerGabinete(tipoPorudcto);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Volviendo durante %d segundos.", volviendo);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);
    usleep(volviendo * 1000 * 1000);
    
    return gabinete;
}

int main(int argc, char **argv) {
    
    Logger::getInstance().setProcessInformation("Robot 5:");
    
    char buffer[TAM_BUFFER];
    
    Logger::getInstance().logMessage(Logger::TRACE, "Iniciando Robot 5.");
    
    ControladorRobot5 controladorRobot5;
    try {
        controladorRobot5.iniciarControlador();
    }
    catch (std::exception ex) {
        sprintf(buffer, "Error al iniciar el controlador: %s", ex.what());
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
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
            Logger::getInstance().logMessage(Logger::TRACE,"Espero por un pedido.");
            PedidoRobot5 pedido = controladorRobot5.obtenerPedido();
                        
            switch (pedido.tipo) {
                case PEDIDO_PRODUCCION:
                    Logger::getInstance().logMessage(Logger::TRACE, "Recibi un pedido de produccion.");
                    sprintf(buffer, "\tOrden de compra: %d Producto: %d Cantidad: %d Diferencia minima: %d.",pedido.pedidoProduccion.nroOrdenCompra, pedido.pedidoProduccion.tipo,pedido.pedidoProduccion.cantidad, pedido.pedidoProduccion.diferenciaMinima);
                    Logger::getInstance().logMessage(Logger::TRACE, buffer);
                    
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
                    Logger::getInstance().logMessage(Logger::TRACE, "Recibi un pedido de GABINETE.");

                    if (productoAProducir.cantidad > 0) {
                        Gabinete gabinete = resolverPedidoGabinete(controladorRobot5, productoAProducir.tipo);
                        int ordenDeCompra = (productoAProducir.cantidad <= productoAProducir.diferenciaMinima) ? 0 : productoAProducir.nroOrdenCompra;
                        productoAProducir.cantidad--;
                        bool ultimo = (productoAProducir.cantidad == 0);
                        
                        ProductoEnProduccion productoEnProduccion;
                        productoEnProduccion.tipoProducto = productoAProducir.tipo;
                        productoEnProduccion.gabinete = gabinete;
                        productoEnProduccion.nroOrdenCompra = ordenDeCompra;
                        productoEnProduccion.falla = false;

                        sprintf(buffer, "Enviando el gabinete de tipo %d para la orden %d.", gabinete.tipoGabinete, ordenDeCompra);
                        Logger::getInstance().logMessage(Logger::TRACE, buffer);

                        controladorRobot5.resolverPedido(productoEnProduccion, ultimo);
                    } else {
                        Logger::getInstance().logMessage(Logger::ERROR, "Recibi un pedido de Gabinete cuando ya se deberían haber fabricado todos lo de la orden de producción.");

                        /* ERROR: Se recibio un pedido para un gabinete, cuando
                         * ya se deberían haber fabricado todos lo de la orden
                         * de producción.
                         */
                    }
                    break;
                case PEDIDO_CANASTO:
                    Canasto canasto;
                    sprintf(buffer, "Recibi un pedido de CANASTO del agv %d y pieza %d.", pedido.pedidoCanastoAgv.idAgv, pedido.pedidoCanastoAgv.tipoPieza);
                    Logger::getInstance().logMessage(Logger::TRACE, buffer);
                    canasto = resolverPedidoCanasto(controladorRobot5, pedido.pedidoCanastoAgv);
                    
                    sprintf(buffer, "Enviando un CANASTO con %d piezas al AGV %d.", canasto.cantidadPiezas, pedido.pedidoCanastoAgv.idAgv);
                    Logger::getInstance().logMessage(Logger::TRACE, buffer);
                    
                    controladorRobot5.resolverPedido(canasto, pedido.pedidoCanastoAgv.idAgv);
                    break;
                    
                default:
                    break;
            }
        }        
        catch (std::exception ex) {
            sprintf(buffer, "Error: %s\n", ex.what());
            Logger::getInstance().logMessage(Logger::ERROR, buffer);
            deboSeguir = false;
        }
    }
    
    Logger::getInstance().logMessage(Logger::TRACE, "Finalizado");
 
    return 0;
}
