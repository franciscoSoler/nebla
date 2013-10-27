#ifndef CONTROLLERROBOT11_H
#define	CONTROLLERROBOT11_H

#include <API/Robot11/IControllerRobot11.h>
#include <CSO/IPCs/Semaphore.h>
#include <CSO/IPCs/SharedMemory.h>
#include <Objects/DataSM_R11_R14.h>
#include <definedTypes.h>

class ControllerRobot11 : public IControllerRobot11 {
    
public:
    ControllerRobot11(uint nroCinta);
    Caja tomarCaja();
    void depositarCaja(Caja unaCaja);
    virtual ~ControllerRobot11();
    
private:
    ControllerRobot11(const ControllerRobot11& orig);
    bool estaRobot14TrabajandoEnEstaCinta() const;
    bool estaCintaLlena() const;
    void bloquearRobot();
    void obtener_shMem_R11_R14();
    void liberar_shMem_R11_R14();
    
private:        
    char buffer_[255];
    uint nroCinta_;
    
    DataSM_R11_R14* shMem_R11_R14_Data_;
    IPC::SharedMemory<DataSM_R11_R14> shMem_R11_R14_;
    IPC::Semaphore semMutex_shMem_R11_R14_;
    IPC::Semaphore semR11_Cinta13_;
    IPC::Semaphore semR14_;
};

#endif	/* CONTROLLERROBOT11_H */

