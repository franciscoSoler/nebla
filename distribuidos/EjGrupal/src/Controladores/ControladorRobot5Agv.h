/*
 * File:   ControladorRobot5Agv.h
 * Author: knoppix
 *
 * Created on October 13, 2013, 9:45 PM
 */

#ifndef CONTROLADORROBOT5AGV_H
#define	CONTROLADORROBOT5AGV_H

#include "../IPC/Semaphore/Semaphore.h"
#include "../IPC/MessageQueue/PedidosAgvMessageQueue.h"
#include "../IPC/SharedMemory/BufferCanastoSharedMemory.h"

#include "../common.h"

class ControladorRobot5Agv {
public:
    ControladorRobot5Agv();
    virtual ~ControladorRobot5Agv();

    void iniciarControlador();
    
    PedidoCanasto obtenerPedido();
    void resolverPedido(Canasto canasto, IDAgv idDestino);

private:
    PedidosAgvMessageQueue colaPedidos;
    Semaphore semaforoSincronizacionRobot5;
    
    /*Semaphore semaforoAgv1;
    Semaphore semaforoAgv2;
    Semaphore semaforoAgv3;
    
    Semaphore semaforoAccesoBufferAgv1;
    Semaphore semaforoAccesoBufferAgv2;
    Semaphore semaforoAccesoBufferAgv3;
    
    BufferCanastoSharedMemory bufferCanasto1;
    BufferCanastoSharedMemory bufferCanasto2;
    BufferCanastoSharedMemory bufferCanasto3;*/
    
    BufferCanastoSharedMemory *bufferCanasto[3];
    Semaphore *semaforoAccesoBufferAgv[3];
    Semaphore *semaforoAgv[3];
};

#endif	/* CONTROLADORROBOT5AGV_H */

