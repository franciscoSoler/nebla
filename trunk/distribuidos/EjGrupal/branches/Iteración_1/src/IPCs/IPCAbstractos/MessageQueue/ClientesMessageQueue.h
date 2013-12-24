/* 
 * File:   ClientesMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:44 PM
 */

#ifndef CLIENTESMESSAGEQUEUE_H
#define	CLIENTESMESSAGEQUEUE_H

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {

class ClientesMessageQueue : public AbstractMessageQueue 
{
public:

    ClientesMessageQueue(std::string IPCName = "") : AbstractMessageQueue(IPCName) {}

    virtual ~ClientesMessageQueue() {}

    int enviarMensajeRespuesta(msg_respuesta_pedido_t dato) {
        return this->enviar ((const void *) &dato, sizeof (msg_respuesta_pedido_t) - sizeof (long)); 
    }

    int recibirMensajeRespuesta(int tipo, msg_respuesta_pedido_t* buffer) {
        return AbstractMessageQueue::recibir(tipo, (void *)buffer, sizeof (msg_respuesta_pedido_t) - sizeof (long));
    }


};

}

#endif	/* CLIENTESMESSAGEQUEUE_H */

