#ifndef CONTROLLER_R16_DESPACHO_H_
#define	CONTROLLER_R16_DESPACHO_H_

#include <API/Robot16_Despacho/IControllerR16_Despacho.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <API/Objects/DataSM_R14_R16.h>
#include <definedTypes.h>
#include <Common.h>

// IPCs de Fede
#include <API/Objects/SmMemAlmacenProductosTerminados.h>

class ControllerR16_Despacho : public IControllerR16_Despacho {
    
public:
    ControllerR16_Despacho();
    PedidoDespacho recibirPedido();
    void tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja);
    void enviarCajaAlCliente(long idCliente, Caja unaCaja);
    
    bool avanzarCinta();
    
    virtual ~ControllerR16_Despacho();
    
private:
    void obtener_shMem_R14_R16();
    void liberar_shMem_R14_R16();
    
private:
    char buffer_[255];
    
    DataSM_R14_R16* shMem_R14_R16_Data_;
    SmMemAlmacenProductosTerminados shMem_APT_;
    
    IPC::SharedMemory<DataSM_R14_R16> shMem_R14_R16_;
    IPC::Semaphore semMutex_shMem_R14_R16_;
    IPC::Semaphore semMutex_shMem_APT_;
    IPC::MsgQueue inputQueueR16_Despacho_;
    IPC::MsgQueue inputQueueDespacho_;
    IPC::MsgQueue R16_Cliente_Queue_;
    Msg_PedidoDespacho mensaje_;
};

#endif	/* CONTROLLER_R16_DESPACHO_H */
