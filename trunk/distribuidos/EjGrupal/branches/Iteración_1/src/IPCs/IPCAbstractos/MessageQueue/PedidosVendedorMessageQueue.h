/* 
 * File:   PedidosVendedorMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:48 PM
 */

#ifndef PEDIDOSVENDEDORMESSAGEQUEUE_H
#define	PEDIDOSVENDEDORMESSAGEQUEUE_H



#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {
    
class PedidosVendedorMessageQueue : public AbstractMessageQueue 
{
public:

    PedidosVendedorMessageQueue(std::string IPCName = "") : AbstractMessageQueue(IPCName) {}

    virtual ~PedidosVendedorMessageQueue() {}

    int enviarMensajePedido(msg_pedido_t dato) {
        return this->enviar ((const void *) &dato, sizeof (msg_pedido_t) - sizeof (long)); 
    }

    int recibirMensajePedido(int tipo, msg_pedido_t* buffer) {
        return AbstractMessageQueue::recibir(tipo, (void *)buffer, sizeof (msg_pedido_t) - sizeof (long));
    }

};

}


#endif	/* PEDIDOSVENDEDORMESSAGEQUEUE_H */

