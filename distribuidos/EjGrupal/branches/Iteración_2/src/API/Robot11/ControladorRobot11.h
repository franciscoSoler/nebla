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
#include <Comunicaciones/Objects/CommSemaphoreMutex.h>
#include "../../IPCs/IPCAbstractos/MessageQueue/Barrera1112MessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"
#include "../Objects/DataSM_R11_R14.h"
#include "../../IPCs/IPCTemplate/SharedMemory.h"
#include <definedTypes.h>

class ControladorRobot11 : public IControladorRobot11 {
public:
    ControladorRobot11();
    virtual ~ControladorRobot11();
    
    void iniciarControlador(int numRobot);
    bool buscarProximoGabinete(EspecifProd *piezas);
    void avanzarCinta();
    BufferCanastos obtenerBufferCanastos();
    //bool agregarPantalla(EspecifPiezaDeProd piezaDeProd);
    void posicionarCanasto(BufferCanastos canastos);
    void pedirPiezaFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza);
    void pedirCanastoFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza);
    void pedirPiezaAlAGV(TipoPieza tipoPieza, int posicionPieza);
    Caja cerrarYTomarCaja();
    void depositarCaja(Caja unaCaja);
        
private:
    char buffer[255];
    int id_Agv;
    uint nroCinta_;
    int id_semMemCanastos;
    int id_Robot;
    //int posicionPieza;
    IPC::CommSemaphore semBloqueoRobot11;
    IPC::CommSemaphoreMutex semBufferCanastos;
    IPC::CommSemaphoreMutex semBufferCinta6;
    IPC::CommSemaphoreMutex semMemEstadoRobot5;
    IPC::CommSemaphore semBloqueoRobot5;

    IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
    IPC::Barrera1112MessageQueue cola11_A_12;
    IPC::Barrera1112MessageQueue cola12_A_11;
    IPC::BufferCanastosSharedMemory shMemBufferCanastos;
    IPC::Cinta6SharedMemory shMemBufferCinta6;
    IPC::EstadoRobot5SharedMemory shMemEstadoRobot5;
    
    DataSM_R11_R14* shMem_R11_R14_Data_;
    IPC::SharedMemory<DataSM_R11_R14> shMem_R11_R14_;
    IPC::CommSemaphoreMutex semMutex_shMem_R11_R14_;
    IPC::CommSemaphore semR11_Cinta13_;
    IPC::CommSemaphore semR14_Cinta13_;
    
    bool poseePieza(int id_pieza);
    //void buscarPosicionPieza(BufferCanastos canastos, int id_pieza);
    void obtenerPantallaDelProducto(TipoProducto tipoProducto, EspecifProd *piezas);
    bool estaRobot14TrabajandoEnEstaCinta() const;
    bool estaCintaLlena() const;
    void bloquearRobot();
    void obtener_shMem_R11_R14();
    void liberar_shMem_R11_R14();

    void imprimirCinta(bool lugarVacio[BUFF_SIZE_CINTA_6], int puntoFinal);
};

#endif	/* CONTROLADORROBOT11_H */

