/* 
 * File:   VendedoresMessageQueue.h
 * Author: eze
 *
 * Created on November 4, 2013, 8:46 PM
 */

#ifndef VENDEDORESMESSAGEQUEUE_H
#define	VENDEDORESMESSAGEQUEUE_H

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {
    
class VendedorLibreMessageQueue : public AbstractMessageQueue
{
public:

    VendedorLibreMessageQueue(std::string IPCName = "") : AbstractMessageQueue(IPCName) {}

    virtual ~VendedorLibreMessageQueue() {}

    int enviarMensajeInicial(mensaje_inicial_t dato) {
        return this->enviar ((const void *) &dato, sizeof (mensaje_inicial_t) - sizeof (long)); 
    }

    int recibirMensajeInicial(int tipo, mensaje_inicial_t* buffer) {
        return AbstractMessageQueue::recibir(tipo, (void *)buffer, sizeof (mensaje_inicial_t) - sizeof (long));
    }


};

}

#endif	/* VENDEDORESMESSAGEQUEUE_H */

