/* 
 * File:   ControladorRobot5Agv.cpp
 * Author: nebla
 * 
 * Created on November 9, 2013, 12:14 PM
 */

#include "ControladorRobot5Agv.h"

#include "Logger/Logger.h"
#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControladorRobot5Agv::ControladorRobot5Agv() : 
        colaPedidos("PedidosAgvMessageQueue", 1, ID_TIPO_ROBOT5_AGV, ID_TIPO_AGV),
        semaforoAccesoBufferAgv("Acceso al buffer AGV - 5"),
        semaforoBloqueoAgv("Bloqueo AGV"),
        semaforoApiRobot5("Api Robot 5")
{

    MiddlewareAPI middleware;
    middleware.crearCanales(1, ID_TIPO_ROBOT5_AGV);

    char buffer[TAM_BUFFER];
    for (int i = 0; i < CANTIDAD_AGVS; ++i) {
        sprintf(buffer, "BufferCanastoSharedMemory %d", i);
        bufferCanasto[i] = IPC::BufferCanastoEntre5yAGVSharedMemory(buffer);
    }
}

ControladorRobot5Agv::~ControladorRobot5Agv() {

}

void ControladorRobot5Agv::iniciarControlador()
{
    try
    {
	/* Obtengo la cola de pedidos */
    /*colaPedidos = IPC::PedidosAgvMessageQueue("colaPedidos", 1, ID_TIPO_AP);*/
	colaPedidos.getMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_AGV_5);

	/* Obtengo el buffer para depositar los canastos */
	bufferCanasto[0].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_0);
    semaforoAccesoBufferAgv.setShMem(DIRECTORY_AGV, ID_BUFFER_AGV_5_0, 0);
	bufferCanasto[1].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_1);
    semaforoAccesoBufferAgv.setShMem(DIRECTORY_AGV, ID_BUFFER_AGV_5_1, 1);
	bufferCanasto[2].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_2);
    semaforoAccesoBufferAgv.setShMem(DIRECTORY_AGV, ID_BUFFER_AGV_5_2, 2);

	/* Obtengo los semaforos de acceso a los buffer */
	semaforoAccesoBufferAgv.getSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, CANTIDAD_AGVS);

	/* Obtengo los semaforos de bloqueo de los Agv */
	semaforoBloqueoAgv.getSemaphore(DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, CANTIDAD_AGVS);

	/* Obtengo el semaforo para la api del robot 5 */
	semaforoApiRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_SEM_API_ROBOT_5, 1);

	almacen.iniciarEspacioAlmacen();
    }    
    catch(Exception &e)
    {
	Logger::logMessage(Logger::TRACE, e.what());
	exit(-1);
    }
}
    
PedidoCanastoAGV ControladorRobot5Agv::obtenerPedidoCanasto() {
    try {
        MensajePedidoAgv_5 nuevoPedido;
        colaPedidos.recibirPedidoAgv(TIPO_PEDIDO_CANASTO, &nuevoPedido);
        
        /* Le envio el pedido al robot 5 Aplicacion */            
        Logger::getInstance().logMessage(Logger::TRACE , "Controlador Robot 5-Agv: Recibió un pedido de un AGV y lo envio al robot 5.");
        
        semaforoApiRobot5.wait();
        return nuevoPedido.pedidoCanastoAgv;
    }
    catch (Exception const& ex) {
	char buffer[TAM_BUFFER];
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
        semaforoApiRobot5.signal();
    }
    catch (Exception const& ex) {
        sprintf(buffer, "Error: %s", ex.what());
        Logger::getInstance().logMessage(Logger::ERROR,buffer);
        exit(-1);
    }
}

Canasto ControladorRobot5Agv::obtenerCanasto(TipoPieza tipoPieza) {
    try {
        Canasto canasto;
        canasto.tipoPieza = tipoPieza;
        canasto.cantidadPiezas = 15;
	//return almacen.obtenerCanastoConPiezas(tipoPieza);
        return canasto;
    }
    catch (Exception const& ex) {
	char buffer[TAM_BUFFER];
        sprintf(buffer, "Error: %s", ex.what());
        Logger::getInstance().logMessage(Logger::ERROR,buffer);
        exit(-1);
    }
}
    
/* Metodos privados, sin implementacion, para evitar copias del objeto */
ControladorRobot5Agv::ControladorRobot5Agv(const ControladorRobot5Agv& orig) {

}

ControladorRobot5Agv& ControladorRobot5Agv::operator= (const ControladorRobot5Agv &p) {
    return *this;
}
