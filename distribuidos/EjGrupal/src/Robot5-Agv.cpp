/* 
 * File:   Robot5-AGV.cpp
 * Author: knoppix
 *
 * Created on November 9, 2013, 12:00 PM
 */

#include <cstdlib>
#include <iostream>
#include <exception>

#include "API/Robot5/ControladorRobot5Agv.h"
#include "Logger.h"

#define MAX_DEMORA 10 //Demora maxima que tarda el robot 5 en ir a buscar un canasto

Canasto resolverPedidoCanasto(ControladorRobot5Agv &controladorRobot5Agv, PedidoCanastoAGV pedido) {
    // El robot va a buscar el canasto al almacen de piezas
    char buffer[TAM_BUFFER];
    int buscando = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Buscando durante %d segundos", buscando);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);

    usleep(buscando * 1000 * 1000);    
    Canasto canasto = controladorRobot5Agv.obtenerCanasto(pedido.tipoPieza);

    int volviendo = rand() % MAX_DEMORA + 1;
    sprintf(buffer, "Volviendo durante %d segundos", volviendo);
    Logger::getInstance().logMessage(Logger::TRACE, buffer);
    usleep(volviendo * 1000 * 1000);
    
    return canasto;
}

int main(int argc, char **argv) {
    
    Logger::getInstance().setProcessInformation("Robot 5 - AGV:");
    
    char buffer[TAM_BUFFER];
    Logger::getInstance().logMessage(Logger::TRACE, "Iniciando Robot 5-AGV.");
    
    ControladorRobot5Agv controladorRobot5Agv;
    
    try {
        controladorRobot5Agv.iniciarControlador();
    }
    catch (std::exception ex) {
        sprintf(buffer, "Error al iniciar el controlador: %s", ex.what());
        Logger::getInstance().logMessage(Logger::ERROR, buffer);
        exit(-1);
    }
    
    bool deboSeguir = true;

    while (deboSeguir) {
        try {
            /* Obtengo un pedido de canasto de un agv para el robot.
             */
            Logger::getInstance().logMessage(Logger::TRACE,"Espero por un pedido de un canasto.");
            
            PedidoCanastoAGV pedidoCanastoAgv = controladorRobot5Agv.obtenerPedidoCanasto();
            
            sprintf(buffer, "Recibi un pedido de CANASTO del agv %d y pieza %d.", pedidoCanastoAgv.idAgv, pedidoCanastoAgv.tipoPieza);
            Logger::getInstance().logMessage(Logger::TRACE, buffer);
            
            Canasto canasto;
            canasto = resolverPedidoCanasto(controladorRobot5Agv, pedidoCanastoAgv);
                    
            sprintf(buffer, "Enviando un CANASTO con %d piezas al AGV %d.", canasto.cantidadPiezas, pedidoCanastoAgv.idAgv);
            Logger::getInstance().logMessage(Logger::TRACE, buffer);
                    
            controladorRobot5Agv.resolverPedidoCanasto(canasto, pedidoCanastoAgv.idAgv);
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