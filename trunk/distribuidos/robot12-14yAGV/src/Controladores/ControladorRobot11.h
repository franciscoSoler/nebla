/* 
 * File:   ControladorRobot11.h
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 13:00
 */

#ifndef CONTROLADORROBOT11_H
#define	CONTROLADORROBOT11_H

#include "../API/IControladorRobot11.h"
#include "../IPC/SharedMemory/EstadoRobot5SharedMemory.h"
#include "../IPC/SharedMemory/Cinta6SharedMemory.h"
#include "../IPC/SharedMemory/BufferCanastoSharedMemory.h"
#include "../IPC/Semaphore/Semaphore.h"
#include "../IPC/MessageQueue/Barrera1112MessageQueue.h"
#include "../IPC/MessageQueue/PedidosCanastosMessageQueue.h"

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
    IDAgv id_Agv;
    int id_semMemCanastos;
    int id_Robot;
    int posicionPieza;
    IPC::Semaphore semRobot11;
    IPC::Semaphore semMemCanastos;
    IPC::Semaphore semMemCinta6;
    IPC::Semaphore semMemEstadoRobot5;
    IPC::Semaphore semEstadoRobot5;

    PedidosCanastosMessageQueue colaPedidosCanastos;
    Barrera1112MessageQueue cola1112;
    Barrera1112MessageQueue cola1211;
    BufferCanastoSharedMemory shMemCanastos;
    Cinta6SharedMemory shMemCinta6;
    EstadoRobot5SharedMemory shMemEstadoRobot5;
    
    bool poseePieza(int id_pieza);
    void buscarPosicionPieza(BufferCanastos canastos, int id_pieza);
};

#endif	/* CONTROLADORROBOT11_H */

