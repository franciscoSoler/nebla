/* 
 * File:   ControladorRobot5Agv.h
 * Author: nebla
 *
 * Created on November 9, 2013, 12:14 PM
 */

#ifndef CONTROLADORROBOT5AGV_H
#define	CONTROLADORROBOT5AGV_H

#include "../../Common.h"

#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosAgvMessageQueue.h"

class ControladorRobot5Agv {

public:
    ControladorRobot5Agv();
    
    virtual ~ControladorRobot5Agv();
   
    void iniciarControlador();
    
    PedidoCanastoAGV obtenerPedidoCanasto();
    
    void resolverPedidoCanasto(Canasto canasto, int idAgvDestino);
    
    Canasto obtenerCanasto(TipoPieza tipoPieza); // Obtiene canasto del almacen de piezas
   
private:
    
    IPC::PedidosAgvMessageQueue colaPedidos;
    IPC::BufferCanastoEntre5yAGVSharedMemory bufferCanasto[CANTIDAD_AGVS]; 
    IPC::Semaphore semaforoAccesoBufferAgv; 
    IPC::Semaphore semaforoBloqueoAgv;
    IPC::Semaphore semaforoApiRobot5;
    
    ControladorRobot5Agv(const ControladorRobot5Agv& orig);
    ControladorRobot5Agv& operator= (const ControladorRobot5Agv &p);
};

#endif	/* CONTROLADORROBOT5AGV_H */