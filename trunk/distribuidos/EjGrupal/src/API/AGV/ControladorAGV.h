/* 
 * File:   ControladorAGV.h
 * Author: francisco
 *
 * Created on 26 de octubre de 2013, 20:26
 */

#ifndef CONTROLADORAGV_H
#define	CONTROLADORAGV_H

#include "../API/IControladorAGV.h"
#include "../IPC/Semaphore/Semaphore.h"
#include "../IPC/MessageQueue/PedidosCanastosMessageQueue.h"
#include "../IPC/MessageQueue/PedidosAGVMessageQueue.h"
#include "../IPC/SharedMemory/BufferCanastoSharedMemory.h"
#include "../IPC/SharedMemory/EstadoRobot5SharedMemory.h"
#include "../IPC/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"

class ControladorAGV : public IControladorAGV {
public:
    ControladorAGV();
    virtual ~ControladorAGV();
    
    void iniciarControlador(IDAgv id_AGV);
    TipoPieza atenderPedidos();
    Canasto buscarPieza(TipoPieza tipoPieza);
    void reponerCanasto(Canasto canasto);
    
private:
    char buffer[255];
    IDAgv id_AGV;
    int id_semMemCanastos;
    int posicionCanasto;
    //bool pedidoDeDeposito;
    
    IPC::Semaphore semEstadoRobot5;
    IPC::Semaphore semMemEstadoRobot5;
    IPC::Semaphore semMemAGV_5;
    IPC::Semaphore semMemCanastos;
    IPC::Semaphore semRobotCinta;
    IPC::Semaphore semEstAGV;

    PedidosCanastosMessageQueue colaPedidosReponerCanastos;
    PedidosAGVMessageQueue colaPedidosAGV;
    BufferCanastoSharedMemory shMemCanastos;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV;
    EstadoRobot5SharedMemory shMemEstadoRobot5;
};

#endif	/* CONTROLADORAGV_H */

