/* 
 * File:   ControladorRobot12.h
 * Author: francisco
 *
 * Created on 24 de octubre de 2013, 22:04
 */

#ifndef CONTROLADORROBOT12_H
#define	CONTROLADORROBOT12_H

#include "../API/IControladorRobot12.h"
#include "../IPC/SharedMemory/Cinta6SharedMemory.h"
#include "../IPC/SharedMemory/BufferCanastoSharedMemory.h"
#include "../IPC/Semaphore/Semaphore.h"
#include "../IPC/MessageQueue/Barrera1112MessageQueue.h"
#include "../IPC/MessageQueue/PedidosCanastosMessageQueue.h"


class ControladorRobot12 : public IControladorRobot12 {
public:
    ControladorRobot12();
    virtual ~ControladorRobot12();
    
    void iniciarControlador(int numRobot);
    EspecifProd esperarProximoGabinete();
    bool agregarConector(EspecifPiezaDeProd piezaDeProd);
    void pedirPiezaAlAGV(TipoPieza tipoPieza);
    void finalizarEnsamble();
    
private:
    char buffer[255];
    IDAgv id_Agv;
    int id_semMemCanastos;
    int id_Robot;
    int posicionPieza;
    int posEsperaDelOtroRobot12;
    IPC::Semaphore semRobot12;
    IPC::Semaphore semMemCinta6;
    IPC::Semaphore semMemCanastos;

    PedidosCanastosMessageQueue colaPedidosCanastos;
    Barrera1112MessageQueue cola1112;
    Barrera1112MessageQueue cola1211;
    BufferCanastoSharedMemory shMemCanastos;
    Cinta6SharedMemory shMemCinta6;
    
    bool poseePieza(int id_pieza);
    void buscarPosicionPieza(BufferCanastos canastos, int id_pieza);
};

#endif	/* CONTROLADORROBOT12_H */

