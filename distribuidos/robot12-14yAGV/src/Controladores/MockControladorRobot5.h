/* 
 * File:   MockControladorRobot5.h
 * Author: francisco
 *
 * Created on 31 de octubre de 2013, 16:14
 */

#ifndef MOCKCONTROLADORROBOT5_H
#define	MOCKCONTROLADORROBOT5_H

#include "../API/IControladorRobot5.h"
#include "../IPC/SharedMemory/Cinta6SharedMemory.h"
#include "../IPC/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "../IPC/SharedMemory/EstadoRobot5SharedMemory.h"
#include "../IPC/Semaphore/Semaphore.h"
#include "../IPC/MessageQueue/PedidosAGVMessageQueue.h"


class MockControladorRobot5 : public IControladorRobot5 {
public:
    MockControladorRobot5();
    void iniciarControlador();
    void atenderPedidoAGV();
    void llenarCinta();
    virtual ~MockControladorRobot5();
private:
    char buffer[255];
    IPC::Semaphore semRobot11;
    IPC::Semaphore semMemCinta6;
    IPC::Semaphore semMemEstadoRobot5;
    IPC::Semaphore semEstadoRobot5;
    IPC::Semaphore semMemAGV_5;
    IPC::Semaphore semEstAGV;
    
    Cinta6SharedMemory shMemCinta61;
    Cinta6SharedMemory shMemCinta62;
    EstadoRobot5SharedMemory shMemEstadoRobot5;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV1;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV2;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV3;

    PedidosAGVMessageQueue colaPedidosAGV;
};

#endif	/* MOCKCONTROLADORROBOT5_H */

