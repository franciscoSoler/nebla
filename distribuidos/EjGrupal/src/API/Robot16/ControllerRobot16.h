#ifndef CONTROLLERROBOT16_H
#define	CONTROLLERROBOT16_H

#include <API/Robot16/IControllerRobot16.h>
#include <CSO/IPCs/Semaphore.h>
#include <CSO/IPCs/SharedMemory.h>
#include <CSO/IPCs/MsgQueue.h>
#include <Objects/DataSM_R14_R16.h>
#include <definedTypes.h>
#include <common.h>

// IPCs de Fede
#include <Objects/SmMemAlmacenProductosTerminados.h>

class ControllerRobot16 : public IControllerRobot16 {
    
public:
    ControllerRobot16();
    PedidoDespacho recibirPedido();
    bool moverCinta();
    bool tomarCajaCinta15(Caja & unaCaja);
    bool depositarCajaEnAPT(Caja unaCaja, long & idNroOrdenAPT);
    void informarAlDespachoProductoTerminado(long idnroOrdenAPT, TipoProducto tipo);
    void tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja);
    void enviarCajaAlCliente(long idCliente, Caja unaCaja);
    
    bool avanzarCinta();
    
    virtual ~ControllerRobot16();
    
private:
    void obtener_shMem_R14_R16();
    void liberar_shMem_R14_R16();
    
private:
    char buffer_[255];
    
    DataSM_R14_R16* shMem_R14_R16_Data_;
    SmMemAlmacenProductosTerminados shMem_APT_;
    
    IPC::SharedMemory<DataSM_R14_R16> shMem_R14_R16_;
    IPC::Semaphore semMutex_shMem_R14_R16_;
    IPC::Semaphore semR14_;
    IPC::Semaphore semR16_;
    IPC::Semaphore semMutex_shMem_APT;
    IPC::MsgQueue outputQueueR16_;
    IPC::MsgQueue outputQueueDespacho_;
    IPC::MsgQueue R16_Cliente_Queue_;
    PedidoDespacho pedido_;
};

#endif	/* CONTROLLERROBOT16_H */