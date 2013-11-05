#ifndef COMUNICACIONROBOT5_MESSAGEQUEUE_H_
#define COMUNICACIONROBOT5_MESSAGEQUEUE_H_

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {

class ComunicacionRobot5MessageQueue : public AbstractMessageQueue 
{
public:

    ComunicacionRobot5MessageQueue(std::string IPCName = "") : AbstractMessageQueue(IPCName) {}

    virtual ~ComunicacionRobot5MessageQueue() {}

    int enviarPedidoRobot5(MensajePedidoRobot5 dato) {
        return this->enviar ((const void *) &dato, sizeof (MensajePedidoRobot5) - sizeof (long)); 
    }

    int recibirPedidoRobot5(int tipo, MensajePedidoRobot5* buffer) {
        return AbstractMessageQueue::recibir(tipo, (void *)buffer, sizeof (MensajePedidoRobot5) - sizeof (long));
    }

    int enviarCanasto(MensajeRespuestaCanasto dato) {
        return AbstractMessageQueue::enviar((const void *) &dato, sizeof (MensajeRespuestaCanasto) - sizeof (long)); 
    }

    int recibirCanasto(int tipo, MensajeRespuestaCanasto* buffer) {
        return AbstractMessageQueue::recibir(tipo, (void *)buffer, sizeof (MensajeRespuestaCanasto) - sizeof (long));
    }
    
    int enviarGabinete(MensajeRespuestaGabinete dato) {
        return AbstractMessageQueue::enviar((const void *) &dato, sizeof (MensajeRespuestaGabinete) - sizeof (long)); 
    }

    int recibirGabinete(int tipo, MensajeRespuestaGabinete* buffer) {
        return AbstractMessageQueue::recibir(tipo, (void *)buffer, sizeof (MensajeRespuestaGabinete) - sizeof (long));
    }
};

}

#endif // COMUNICACIONROBOT5_MESSAGEQUEUE_H_

