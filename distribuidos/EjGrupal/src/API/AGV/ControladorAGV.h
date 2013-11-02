/* 
 * File:   ControladorAGV.h
 * Author: francisco
 *
 * Created on 26 de octubre de 2013, 20:26
 */

#ifndef CONTROLADORAGV_H
#define	CONTROLADORAGV_H

#include "IControladorAGV.h"
#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/Barrera1112MessageQueue.h"

#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosAgvMessageQueue.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"

class ControladorAGV : public IControladorAGV {
public:
    ControladorAGV();
    virtual ~ControladorAGV();
    
    void iniciarControlador(int id_AGV);
    TipoPieza atenderPedidos();
    Canasto buscarPieza(TipoPieza tipoPieza);
    void reponerCanasto(Canasto canasto);
    
private:
    char buffer[255];
    int id_AGV;
    int id_semMemCanastos;
    int posicionCanasto;
    
    IPC::Semaphore semBloqueoRobot5;
    IPC::Semaphore semMemEstadoRobot5;
    IPC::Semaphore semBufferAGV_5;
    IPC::Semaphore semMemCanastos;
    IPC::Semaphore semRobotCinta;
    IPC::Semaphore semBloqueoAGV;

    IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
    IPC::PedidosAgvMessageQueue colaPedidosAGV_5;
    IPC::BufferCanastosSharedMemory shMemBufferCanastos;
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemBuffer5yAGV;
    IPC::EstadoRobot5SharedMemory shMemEstadoRobot5;
};

#endif	/* CONTROLADORAGV_H */

