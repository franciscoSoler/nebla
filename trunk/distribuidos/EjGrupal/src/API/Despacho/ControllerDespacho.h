#ifndef CONTROLLER_DESPACHO_
#define CONTROLLER_DESPACHO_

#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <API/Despacho/IControllerDespacho.h>
#include <Logger/Logger.h>

// Include de Fede
// #include <Objects/SmMemAlmacenProductosTerminados.h>

class ControllerDespacho : public IControllerDespacho {

public:
    ControllerDespacho();
    virtual ~ControllerDespacho();
    PedidoDespacho recibirPedido();
    OrdenDeCompra obtenerOrdenDeCompra(long idOrdenDeCompra);
    void despacharProducto(TipoProducto producto, long idOrdenCompra);
    void notificarAClienteProductoTerminado(long idCliente, long idOrdenCompra, 
            TipoProducto idProducto);

private:
    char buffer_[255];
    
    // Cola por la cual recibe mensajes de varios procesos
    IPC::MsgQueue outputQueueDespacho_;
    IPC::MsgQueue outputQueueCliente_;
    IPC::MsgQueue outputQueueRobot16_;
};

#endif /* CONTROLLER_DESPACHO_ */

