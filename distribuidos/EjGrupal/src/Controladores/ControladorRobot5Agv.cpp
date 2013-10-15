/* 
 * File:   ControladorRobot5Agv.cpp
 * Author: nebla
 * 
 * Created on October 13, 2013, 9:45 PM
 */

#include "ControladorRobot5Agv.h"

ControladorRobot5Agv::ControladorRobot5Agv() {
}

ControladorRobot5Agv::~ControladorRobot5Agv() {
}

void ControladorRobot5Agv::iniciarControlador() {
    
    /* Obtengo la cola de pedidos */
    colaPedidos = PedidosAgvMessageQueue();
    colaPedidos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
    
    /* Obtengo el semaforo de sincronizacion de Robot5_11 */
    semaforoSincronizacionRobot5 = Semaphore();
    semaforoSincronizacionRobot5.getSemaphore(DIRECTORY, ID_SEM_SINCRONIZACION_5, 1);
           
    /* Obtengo el buffer para depositar los canastos */
    BufferCanastoSharedMemory bufferCanastoAux;
    
    bufferCanastoAux = BufferCanastoSharedMemory();
    bufferCanastoAux.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_1);
    bufferCanasto[0] = &bufferCanastoAux;
        
    bufferCanastoAux = BufferCanastoSharedMemory();
    bufferCanastoAux.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_2);
    bufferCanasto[1] = &bufferCanastoAux;
    
    bufferCanastoAux = BufferCanastoSharedMemory();
    bufferCanastoAux.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_3);
    bufferCanasto[2] = &bufferCanastoAux;
    
    /* Obtengo los semaforos de acceso a los buffer */
    Semaphore semaforoAccesoBufferAux;
    
    semaforoAccesoBufferAux = Semaphore();
    semaforoAccesoBufferAux.getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_CANASTOS_1, 1);
    semaforoAccesoBufferAgv[0] = &semaforoAccesoBufferAux;
            
    semaforoAccesoBufferAux = Semaphore();
    semaforoAccesoBufferAux.getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_CANASTOS_2, 1);
    semaforoAccesoBufferAgv[1] = &semaforoAccesoBufferAux;
            
    semaforoAccesoBufferAux = Semaphore();
    semaforoAccesoBufferAux.getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_CANASTOS_3, 1);
    semaforoAccesoBufferAgv[2] = &semaforoAccesoBufferAux;
    
    /* Obtengo los semaforos de bloqueo de los Agv*/
    Semaphore semaforoAgvAux;

    semaforoAgvAux = Semaphore();
    semaforoAgvAux.getSemaphore(DIRECTORY, ID_SEM_AGV_1, 1);
    semaforoAgv[0] = &semaforoAgvAux;
 
    semaforoAgvAux = Semaphore();
    semaforoAgvAux.getSemaphore(DIRECTORY, ID_SEM_AGV_2, 1);
    semaforoAgv[1] = &semaforoAgvAux;
 
    semaforoAgvAux = Semaphore();
    semaforoAgvAux.getSemaphore(DIRECTORY, ID_SEM_AGV_3, 1);
    semaforoAgv[2] = &semaforoAgvAux;
}

PedidoCanasto ControladorRobot5Agv::obtenerPedido() {
    /* Obtengo un pedido de alguno de los AGV */
    PedidoAgv *nuevoPedido;
    colaPedidos.recibirPedidoAgv(TIPO_PEDIDO_CANASTO, nuevoPedido);
    
    /* Me sincronizo con el robot 5 que se comunica con el robot 11 */
    semaforoSincronizacionRobot5.wait();
    
    /* En este momento el otro robot 5, no esta trabajando, asi que lo puedo hacer yo */
    return (*nuevoPedido).pedidoCanasto;   
}

void ControladorRobot5Agv::resolverPedido(Canasto canasto, IDAgv idDestino) {
    
    /* Intento acceder al buffer correspondiente al agv al que 
     * va destinado al pedido.  
     */
    (*semaforoAccesoBufferAgv[idDestino - 1]).wait();
    
    /* Una vez obtenido al acceso, dejo el canasto correspondiente */
    (*bufferCanasto[idDestino - 1]).writeInfo(&canasto);
    
    /* Libero al AGV correspondiente para que este retire el canasto */
    (*semaforoAgv[idDestino - 1]).signal();
    
    /* Libero el buffer del canasto */
    (*semaforoAccesoBufferAgv[idDestino - 1]).signal();
    
    /* Indico que dejo de trabajar */
    semaforoSincronizacionRobot5.signal();
}
