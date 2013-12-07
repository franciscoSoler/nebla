#ifndef CONTROLLER_R16_DESPACHO_H_
#define	CONTROLLER_R16_DESPACHO_H_

#include <API/Robot16_Despacho/IControllerR16_Despacho.h>
#include <Comunicaciones/Objects/CommSemaphoreMutex.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <API/Objects/DataSM_R14_R16.h>
#include <definedTypes.h>
#include <Common.h>

#include <Comunicaciones/Objects/CommMsgHandler.h>

// IPCs de Fede
#include <API/Objects/SmMemAlmacenProductosTerminados.h>
class ControllerR16_Despacho : public IControllerR16_Despacho {
    
public:
    ControllerR16_Despacho();
    PedidoDespacho recibirPedido(bool & ultimoProductoDeODC);
    void tomarCajaDeAPT(PedidoDespacho pedido, Caja* unaCaja);
    void enviarCajaAlCliente(long idCliente, Caja unaCaja, bool ultimoProductoDeODC);
    
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
    IPC::CommSemaphoreMutex semMutex_shMem_R14_R16_;
    IPC::CommSemaphoreMutex semMutex_shMem_APT_;
    CommMsgHandler inputQueueR16_Despacho_;
    CommMsgHandler R16_Cliente_Queue_;
    Msg_FinProductoR16 mensaje_;
};

#endif	/* CONTROLLER_R16_DESPACHO_H */
