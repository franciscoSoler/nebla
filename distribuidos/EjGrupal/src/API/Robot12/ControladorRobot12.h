/* 
 * File:   ControladorRobot12.h
 * Author: francisco
 *
 * Created on 24 de octubre de 2013, 22:04
 */

#ifndef CONTROLADORROBOT12_H
#define	CONTROLADORROBOT12_H

#include "../../API/Robot12/IControladorRobot12.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/Barrera1112MessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"


class ControladorRobot12 : public IControladorRobot12 {
public:
    ControladorRobot12();
    virtual ~ControladorRobot12();
    
    void iniciarControlador(int numRobot);
    CintaTransportadora_6 esperarProximoGabinete();
    BufferCanastos obtenerBufferCanastos();
    void pedirPiezaFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza);
    void pedirCanastoFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza);
    void posicionarCanasto(BufferCanastos canastos);
    //bool agregarConector(EspecifPiezaDeProd piezaDeProd);
    void pedirPiezaAlAGV(TipoPieza tipoPieza, int posicionPieza);
    void finalizarEnsamble();
    
private:
    char buffer[255];
    int id_Agv;
    int id_semMemCanastos;
    int id_Robot;
    //int posicionPieza;
    int posEsperaDelOtroRobot12;
    IPC::Semaphore semBloqueoRobot12;
    IPC::Semaphore semBufferCinta6;
    IPC::Semaphore semBufferCanastos;

    IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
    IPC::Barrera1112MessageQueue cola11_A_12;
    IPC::Barrera1112MessageQueue cola12_A_11;
    IPC::BufferCanastosSharedMemory shMemBufferCanastos;
    IPC::Cinta6SharedMemory shMemBufferCinta6;
    
    //bool poseePieza(int id_pieza);
    //void buscarPosicionPieza(BufferCanastos canastos, int id_pieza);
    //void obtenerPiezasDelProducto(TipoProducto tipoProducto, EspecifProd *piezas);
};

#endif	/* CONTROLADORROBOT12_H */

