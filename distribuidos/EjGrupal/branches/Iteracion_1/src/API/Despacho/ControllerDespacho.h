#ifndef CONTROLLER_DESPACHO_
#define CONTROLLER_DESPACHO_

#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <API/Despacho/IControllerDespacho.h>
#include <Logger/Logger.h>


class ControllerDespacho : public IControllerDespacho {

public:
    ControllerDespacho();
    virtual ~ControllerDespacho();
    PedidoDespacho recibirPedido();
    OrdenDeCompra obtenerOrdenDeCompra(long idOrdenDeCompra);
    void despacharProducto(TipoProducto producto, uint idOrdenCompra, long idCliente);
    void notificarAClienteProductoTerminado(PedidoDespacho pedido, bool ultimoPedido);

private:
    char buffer_[255];
    
    // Cola por la cual recibe mensajes de varios procesos
    IPC::MsgQueue inputQueueDespacho_;
    // Cola con la cual le envía mensajes al cliente
    IPC::MsgQueue inputQueueCliente_;
    // Cola para avisarle al R16 que tiene que sacar
    // cajas del APT para darselas al cliente
    IPC::MsgQueue inputQueueR16_Despacho_;
};

#endif /* CONTROLLER_DESPACHO_ */

