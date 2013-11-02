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
    
    IPC::Semaphore semEstadoRobot5;
    IPC::Semaphore semMemEstadoRobot5;
    IPC::Semaphore semMemAGV_5;
    IPC::Semaphore semMemCanastos;
    IPC::Semaphore semRobotCinta;
    IPC::Semaphore semEstAGV;

    IPC::PedidosCanastosMessageQueue colaPedidosReponerCanastos;
    IPC::PedidosAgvMessageQueue colaPedidosAGV;
    IPC::BufferCanastosSharedMemory shMemCanastos;
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV;
    IPC::EstadoRobot5SharedMemory shMemEstadoRobot5;
};

#endif	/* CONTROLADORAGV_H */

