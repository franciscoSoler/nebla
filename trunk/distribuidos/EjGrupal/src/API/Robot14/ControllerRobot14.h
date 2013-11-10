#ifndef CONTROLLERROBOT14_H
#define	CONTROLLERROBOT14_H

#include <API/Robot14/IControllerRobot14.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <API/Objects/DataSM_R11_R14.h>
#include <API/Objects/DataSM_R14_R16.h>
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
    void comenzarATrabajar();
    bool estaRobot11BloqueadoEnCinta(uint nroCinta);
    uint obtenerCantidadDeCajasEnCinta(uint nroCinta);
    bool moverCinta(uint nroCinta);
    bool tomarCaja(Caja & unaCaja, uint nroCinta);
    void depositarCaja(Caja unaCaja);

    /* Funciones llamadas por los States */
    bool moverCintaEnEstadoDurmiendo(uint nroCinta);
    bool moverCintaEnEstadoTrabajando(uint nroCinta);
    bool tomarCajaEnEstadoTrabajando(Caja & unaCaja, uint nroCinta);
    void depositarCajaEnEstadoConCarga(Caja & caja);
    
    IStateR14* getEstadoDurmiendo();
    IStateR14* getEstadoConCarga();
    IStateR14* getEstadoTrabajando();
    void setEstado(IStateR14* estado);
    
    virtual ~ControllerRobot14();
    
private:
    ControllerRobot14(const ControllerRobot14& orig);
    bool obtener_shMem_R11_R14();
    bool liberar_shMem_R11_R14();
    void obtener_shMem_R14_R16();
    void liberar_shMem_R14_R16();
    // En este método se encapsula el movimiento de la cinta y las acciones
    // que debe hacer el Robot14 en el caso de que haya podido avanzar. 
    bool avanzarCinta(uint nroCinta);
    void depositarCajaEnCinta15(Caja unaCaja);
    bool estaRobot16Trabajando() const;
    bool estaCinta15Llena() const;
    void bloquearRobot();
    
private:
    char buffer_[255];
    // True si se tomó el mutex de la smMem_R11_R14 y no se liberó el mismo
    bool estaMutex_R11_R14_tomado_;
    
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
    IPC::Semaphore semR14_Cinta13;
    IPC::Semaphore semR14_Cinta15;
    
    // IPC::Semaphore semR16_;
    IPC::MsgQueue inputQueueR16_Cinta15_;
    Msg_AvisoCajaEnCinta15 mensaje_;
};

#endif	/* CONTROLLERROBOT14_H */

