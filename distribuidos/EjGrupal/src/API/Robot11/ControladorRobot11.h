/* 
 * File:   ControladorRobot11.h
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 13:00
 */

#ifndef CONTROLADORROBOT11_H
#define	CONTROLADORROBOT11_H

#include "../../API/Robot11/IControladorRobot11.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/Barrera1112MessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"

class ControladorRobot11 : public IControladorRobot11 {
public:
    ControladorRobot11();
    virtual ~ControladorRobot11();
    
    void iniciarControlador(int numRobot);
    bool buscarProximoGabinete(EspecifProd *piezas);
    void avanzarCinta();
    bool agregarPantalla(EspecifPiezaDeProd piezaDeProd);
    void pedirPiezaAlAGV(TipoPieza tipoPieza);
    Caja cerrarYTomarCaja();
    void depositarCaja(Caja unaCaja);
        
private:
    char buffer[255];
    int id_Agv;
    int id_semMemCanastos;
    int id_Robot;
    int posicionPieza;
    IPC::Semaphore semBloqueoRobot11;
    IPC::Semaphore semMemCanastos;
    IPC::Semaphore semBufferCinta6;
    IPC::Semaphore semMemEstadoRobot5;
    IPC::Semaphore semBloqueoRobot5;

    IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
    IPC::Barrera1112MessageQueue cola11_A_12;
    IPC::Barrera1112MessageQueue cola12_A_11;
    IPC::BufferCanastosSharedMemory shMemBufferCanastos;
    IPC::Cinta6SharedMemory shMemBufferCinta6;
    IPC::EstadoRobot5SharedMemory shMemEstadoRobot5;
    
    bool poseePieza(int id_pieza);
    void buscarPosicionPieza(BufferCanastos canastos, int id_pieza);
};

#endif	/* CONTROLADORROBOT11_H */

