#ifndef CONTROLLERROBOT16_H
#define	CONTROLLERROBOT16_H

#include <API/Robot16/IControllerRobot16.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <Objects/DataSM_R14_R16.h>
#include <API/Robot16/State/StateR16_Durmiendo.h>
#include <API/Robot16/State/StateR16_ConCarga.h>
#include <API/Robot16/State/StateR16_Trabajando.h>
#include <definedTypes.h>
#include <Common.h>

// Forward Declaration
class StateR14_Durmiendo;
class StateR14_Trabajando;
class StateR14_ConCarga;

class ControllerRobot16 : public IControllerRobot16 {
    
public:
    ControllerRobot16();
    bool moverCinta();
    bool tomarCaja(Caja & unaCaja);
    void depositarCaja(Caja unaCaja);
    
    /* Funciones llamadas por los States */
    bool moverCintaEnEstadoDurmiendo();
    bool moverCintaEnEstadoTrabajando();
    bool tomarCajaEnEstadoTrabajando(Caja & unaCaja);
    void depositarCajaEnEstadoConCarga(Caja & caja);
    bool avanzarCinta();
    void depositarCajaEnAPT(Caja caja);
    
    IStateR16* getEstadoDurmiendo();
    IStateR16* getEstadoConCarga();
    IStateR16* getEstadoTrabajando();
    void setEstado(IStateR16* estado);
    
    virtual ~ControllerRobot16();
    
private:
    void obtener_shMem_R14_R16();
    void liberar_shMem_R14_R16();
    
private:
    char buffer_[255];
    
    IStateR16* estadoDurmiendo_;
    IStateR16* estadoTrabajando_;
    IStateR16* estadoConCarga_;
    IStateR16* estadoActual_;
    
    DataSM_R14_R16* shMem_R14_R16_Data_;
    IPC::SharedMemory<DataSM_R14_R16> shMem_R14_R16_;
    IPC::Semaphore semMutex_shMem_R14_R16_;
    IPC::Semaphore semR14_;
    IPC::Semaphore semR16_;
};

#endif	/* CONTROLLERROBOT16_H */