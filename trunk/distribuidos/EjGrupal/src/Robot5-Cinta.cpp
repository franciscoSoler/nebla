/* 
 * File:   Robot5-Cinta.cpp
 * Author: nebla
 *
 * Created on November 9, 2013, 1:22 PM
 */

#include <cstdlib>
#include <iostream>
#include <exception>

#include "Common.h"
#include "Logger.h"

#define MAX_DEMORA 10 //Demora maxima que tarda el robot 5 en ir a buscar un canasto

Gabinete resolverPedidoGabinete(ControladorRobot5Cinta &controladorRobot5Cinta, TipoProducto tipoPorudcto) {
    // El robot va a buscar el gabinete al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Buscando durante %d segundos.", buscando);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);
    usleep(buscando * 1000 * 1000);
    
    Gabinete gabinete = controladorRobot5Cinta.obtenerGabinete(tipoPorudcto);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Volviendo durante %d segundos.", volviendo);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);
    usleep(volviendo * 1000 * 1000);
    
    return gabinete;
}

int obtenerCintaAUtilizar(EstadoCinta estadoCinta0, EstadoCinta estadoCinta1) {
     int cintaAUtilizar = 0;
     if (!estadoCinta0.ocupado && estadoCinta1.ocupado) {
         /* Utilizo la cinta 0. */
         Logger::getInstance().logMessage(Logger::TRACE, "Utilizo la cinta 0, puesto que la 1 esta bloqueada.");
         cintaAUtilizar = 0;
     } else if (estadoCinta0.ocupado && !estadoCinta1.ocupado) {
         /* Utilizo la cinta 1. */
         Logger::getInstance().logMessage(Logger::TRACE, "Utilizo la cinta 1, puesto que la 0 esta bloqueada.");
         cintaAUtilizar = 1;
     } else {
         /* Ambas estan libres, utilizo la que menos gabinetes tenga. */
         cintaAUtilizar = (estadoCinta0.cantOcupados <= estadoCinta1.cantOcupados) ? 0 : 1;
         char buffer[TAM_BUFFER];
         sprintf(buffer, "Ambas cintas estan libres, utilizo %d que es la menos cargada.", cintaAUtilizar);
         Logger::getInstance().logMessage(Logger::TRACE, buffer);
     }
     return cintaAUtilizar;
}

int main(int argc, char **argv) {
    
    Logger::getInstance().setProcessInformation("Robot 5-Cinta:");
    
    char buffer[TAM_BUFFER];
    
    Logger::getInstance().logMessage(Logger::TRACE, "Iniciando Robot 5-Cinta.");
    
    ControladorRobot5Cinta controladorRobot5Cinta;
    try {
        controladorRobot5Cinta.iniciarControlador();
    }
    catch (std::exception ex) {
        sprintf(buffer, "Error al iniciar el controlador: %s", ex.what());
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
        exit(-1);
    }
    
    bool deboSeguir = true;
    
    while (deboSeguir) {
        /* Obtengo un pedido de producción de un producto.
         */
        Logger::getInstance().logMessage(Logger::TRACE,"Espero por un pedido.");
        PedidoProduccion pedidoProduccion = controladorRobot5Cinta.obtenerPedidoProduccion();

        /* Recibi una orden de produccion */
        Logger::getInstance().logMessage(Logger::TRACE, "Recibi un pedido de produccion.");
        sprintf(buffer, "\tOrden de compra: %d Producto: %d Cantidad: %d Diferencia minima: %d.",pedidoProduccion.nroOrdenCompra, pedidoProduccion.tipo, pedidoProduccion.cantidad, pedidoProduccion.diferenciaMinima);
        Logger::getInstance().logMessage(Logger::TRACE, buffer);

        while (pedidoProduccion.cantidad > 0) {
            /* Espero por un pedido de un gabinete, es decir, cuando alguna de las cintas este libre */
            controladorRobot5Cinta.obtenerPedidoGabinete();
            Logger::getInstance().logMessage(Logger::TRACE, "Recibi un pedido de GABINETE.");
            
            Gabinete gabinete = resolverPedidoGabinete(controladorRobot5Cinta, pedidoProduccion.tipo);
            int ordenDeCompra = (pedidoProduccion.cantidad <= pedidoProduccion.diferenciaMinima) ? 0 : pedidoProduccion.nroOrdenCompra;
            pedidoProduccion.cantidad--;
            
            ProductoEnProduccion productoEnProduccion;
            productoEnProduccion.tipoProducto = pedidoProduccion.tipo;
            productoEnProduccion.gabinete = gabinete;
            productoEnProduccion.nroOrdenCompra = ordenDeCompra;
            productoEnProduccion.falla = false;

            sprintf(buffer, "Enviando el gabinete de tipo %d para la orden %d.", gabinete.tipoGabinete, ordenDeCompra);
            Logger::getInstance().logMessage(Logger::TRACE, buffer);

            EstadoCinta estadoCinta0 = controladorRobot5Cinta.obtenerEstadoCinta(0);
            EstadoCinta estadoCinta1 = controladorRobot5Cinta.obtenerEstadoCinta(1);
            
            int cintaAUtilizar = obtenerCintaAUtilizar(estadoCinta0, estadoCinta1);
            
            /* Una vez obtenida la cinta a utilizar, deposito 
             * el producto en produccion 
             */
            controladorRobot5Cinta.resolverPedidoGabinete(productoEnProduccion, cintaAUtilizar);
        }
    }
    
    Logger::getInstance().logMessage(Logger::TRACE, "Finalizado");
 
    return 0;
}