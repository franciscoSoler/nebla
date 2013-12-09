/* 
 * File:   ControladorRobot5Agv.h
 * Author: nebla
 *
 * Created on November 9, 2013, 12:14 PM
 */

#ifndef CONTROLADORROBOT5AGV_H
#define	CONTROLADORROBOT5AGV_H

#include "../../Common.h"

#include <Comunicaciones/Objects/CommSemaphoreMutex.h>
#include "../../IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosAgvMessageQueue.h"

#include "../AlmacenPiezas/EspacioAlmacenPiezas/EspacioAlmacenPiezas.h"

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
    COMM::CommSemaphoreMutex semaforoAccesoBufferAgv; 
    COMM::CommSemaphore semaforoBloqueoAgv;
    IPC::Semaphore semaforoApiRobot5;
    
    EspacioAlmacenPiezas almacen;
    
    ControladorRobot5Agv(const ControladorRobot5Agv& orig);
    ControladorRobot5Agv& operator= (const ControladorRobot5Agv &p);
};

#endif	/* CONTROLADORROBOT5AGV_H */