#ifndef CONTROLLER_DESPACHO_
#define CONTROLLER_DESPACHO_

#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <API/Despacho/IControllerDespacho.h>
#include <Logger/Logger.h>

#include <Comunicaciones/Objects/CommMsgHandler.h>


class ControllerDespacho : public IControllerDespacho {

public:
    ControllerDespacho();
    virtual ~ControllerDespacho();
    PedidoDespacho recibirPedido();
    void despacharProducto(PedidoDespacho pedido, bool ultimoProductoDeODC);
    void notificarAClienteProductoTerminado(PedidoDespacho pedido, bool ultimoPedido);

private:
    char buffer_[255];
    
    // Cola por la cual recibe mensajes de varios procesos
    CommMsgHandler inputQueueDespacho_;
    // Cola con la cual le envía mensajes al cliente
    CommMsgHandler inputQueueCliente_;
    // Cola para avisarle al R16 que tiene que sacar
    // cajas del APT para darselas al cliente
    CommMsgHandler inputQueueR16_Despacho_;
};

#endif /* CONTROLLER_DESPACHO_ */

