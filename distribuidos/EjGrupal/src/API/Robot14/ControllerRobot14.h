#ifndef CONTROLLERROBOT14_H
#define	CONTROLLERROBOT14_H

#include <API/Robot14/IControllerRobot14.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <Objects/DataSM_R11_R14.h>
#include <Objects/DataSM_R14_R16.h>
#include <API/Robot14/State/StateR14_Durmiendo.h>
#include <API/Robot14/State/StateR14_ConCarga.h>
#include <API/Robot14/State/StateR14_Trabajando.h>
#include <definedTypes.h>
#include <Common.h>

// Forward Declaration
class StateR14_Durmiendo;
class StateR14_Trabajando;
class StateR14_ConCarga;

class ControllerRobot14 : public IControllerRobot14 {
public:
    ControllerRobot14();
    bool moverCinta();
    bool tomarCaja(Caja & unaCaja);
    void depositarCaja(Caja unaCaja);

    /* Funciones llamadas por los States */
    bool moverCintaEnEstadoDurmiendo();
    bool moverCintaEnEstadoTrabajando();
    bool tomarCajaEnEstadoTrabajando(Caja & unaCaja);
    void depositarCajaEnEstadoConCarga(Caja & caja);
    
    IStateR14* getEstadoDurmiendo();
    IStateR14* getEstadoConCarga();
    IStateR14* getEstadoTrabajando();
    void setEstado(IStateR14* estado);
    
    virtual ~ControllerRobot14();
    
private:
    ControllerRobot14(const ControllerRobot14& orig);
    uint elegirCinta() const;
    uint elegirCintaPorCantidadDeCajas() const;
    void obtener_shMem_R11_R14();
    void liberar_shMem_R11_R14();
    void obtener_shMem_R14_R16();
    void liberar_shMem_R14_R16();
    void logEleccionCinta(uint nroCinta) const;
    // En este método se encapsula el movimiento de la cinta y las acciones
    // que debe hacer el Robot14 en el caso de que haya podido avanzar. 
    bool avanzarCinta(uint nroCinta);
    void depositarCajaEnCinta15(Caja unaCaja);
    bool estaRobot16Trabajando() const;
    bool estaCinta15Llena() const;
    void bloquearRobot();
    
private:
    char buffer_[255];
    
    DataSM_R11_R14* shMem_R11_R14_Data_;
    DataSM_R14_R16* shMem_R14_R16_Data_;

    IStateR14* estadoDurmiendo_;
    IStateR14* estadoConCarga_;
    IStateR14* estadoTrabajando_;
    IStateR14* estadoActual_;
    
    IPC::SharedMemory<DataSM_R11_R14> shMem_R11_R14_;
    IPC::SharedMemory<DataSM_R14_R16> shMem_R14_R16_;
    IPC::Semaphore semMutex_shMem_R11_R14_;
    IPC::Semaphore semMutex_shMem_R14_R16_;
    IPC::Semaphore semR11_Cinta13_;
    IPC::Semaphore semR14_;
    IPC::Semaphore semR16_;
    
    // IPC::Semaphore semR16_;
    IPC::MsgQueue outputQueueR16_;
    PedidoDespacho pedido_;
};

#endif	/* CONTROLLERROBOT14_H */

