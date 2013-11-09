/* 
 * File:   ControladorRobot5Agv.cpp
 * Author: nebla
 * 
 * Created on November 9, 2013, 12:14 PM
 */

#include "ControladorRobot5Agv.h"

ControladorRobot5Agv::ControladorRobot5Agv() : 
                colaPedidos("PedidosAgvMessageQueue"),
                semaforoAccesoBufferAgv("Acceso al buffer AGV - 5"),
                semaforoBloqueoAgv("Bloqueo AGV")
{
    char buffer[TAM_BUFFER];
    for (int i = 0; i < CANTIDAD_AGVS; ++i) {
        sprintf(buffer, "BufferCanastoSharedMemory %d", i);
        bufferCanasto[i] = IPC::BufferCanastoEntre5yAGVSharedMemory(buffer);
    }
}

virtual ControladorRobot5Agv::~ControladorRobot5Agv() {

}

void ControladorRobot5Agv::iniciarControlador() {
    char buffer[TAM_BUFFER];

    /* Obtengo la cola de pedidos */
    colaPedidos.getMessageQueue(DIRECTORY_AGV,ID_COLA_PEDIDOS_AGV_5);

    /* Obtengo el buffer para depositar los canastos */    
    bufferCanasto[0].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_0);
    bufferCanasto[1].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_1);
    bufferCanasto[2].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_2);
    
    /* Obtengo los semaforos de acceso a los buffer */    
    semaforoAccesoBufferAgv.getSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, CANTIDAD_AGVS);
    
    /* Obtengo los semaforos de bloqueo de los Agv */
    semaforoBloqueoAgv.getSemaphore(DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, CANTIDAD_AGVS);
}
    
PedidoCanastoAGV ControladorRobot5Agv::obtenerPedidoCanasto() {
    char buffer[TAM_BUFFER];
    try {
        MensajePedidoAgv_5 nuevoPedido;
        colaPedidos.recibirPedidoAgv(TIPO_PEDIDO_CANASTO, &nuevoPedido);
        
        /* Le envio el pedido al robot 5 Aplicacion */            
        Logger::getInstance().logMessage(Logger::TRACE , "Controlador Robot 5-Agv: Recibió un pedido de un AGV y lo envio al robot 5.");
            
        return nuevoPedido.pedidoCanastoAgv;
    }
    catch (Exception const& ex) {
        sprintf(buffer, "Error: %s", ex.what());
        Logger::getInstance().logMessage(Logger::ERROR,buffer);
        exit(-1);
    }
}

void ControladorRobot5Agv::resolverPedidoCanasto(Canasto canasto, int idAgvDestino) {
    char buffer[TAM_BUFFER];
    try {
        /* Una vez recibido el canasto requerido, se lo envio al agv correspondiente.
         * Intento acceder al buffer del agv al que va destinado al pedido.  
         */
        semaforoAccesoBufferAgv.wait(idAgvDestino);
        {
            sprintf(buffer, "Controlador Robot 5 - AGV: Accedo al buffer del canasto para el AGV: %d.",idAgvDestino);
            Logger::getInstance().logMessage(Logger::TRACE, buffer);

            /* Una vez obtenido al acceso, dejo el canasto. */
            bufferCanasto[idAgvDestino].writeInfo(&canasto);

            /* Libero al AGV para que este retire el canasto. */
            semaforoAccesoBufferAgv.signal(idAgvDestino);

            sprintf(buffer, "Libero al AGV %d.",idAgvDestino);
            Logger::getInstance().logMessage(Logger::TRACE, buffer);
        }
        /* Libero el buffer del canasto */
        semaforoBloqueoAgv.signal(idAgvDestino);
    }
    catch (Exception const& ex) {
        sprintf(buffer, "Error: %s", ex.what());
        Logger::getInstance().logMessage(Logger::ERROR,buffer);
        exit(-1);
    }
}

Canasto ControladorRobot5Agv::obtenerCanasto(TipoPieza tipoPieza) {

}
    
/* Metodos privados, sin implementacion, para evitar copias del objeto */
ControladorRobot5Agv::ControladorRobot5Agv(const ControladorRobot5Agv& orig) {

}

ControladorRobot5Agv& ControladorRobot5Agv::operator= (const ControladorRobot5Agv &p) {
    return this;
}