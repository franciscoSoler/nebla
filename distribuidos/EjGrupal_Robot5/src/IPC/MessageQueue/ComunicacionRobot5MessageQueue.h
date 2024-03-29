#ifndef COMUNICACIONROBOT5_MESSAGEQUEUE_H_
#define COMUNICACIONROBOT5_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"
#include "../IPCException.h"

#include "../../common.h"

class ComunicacionRobot5MessageQueue : public AbstractMessageQueue 
{
public:

    ComunicacionRobot5MessageQueue() : AbstractMessageQueue() {}

    virtual ~ComunicacionRobot5MessageQueue() {}

    int enviarPedidoRobot5(MensajePedidoRobot5 dato) {
        return this->enviar ("ComunicacionRobot5MessageQueue", (const void *) &dato, sizeof (MensajePedidoRobot5) - sizeof (long)); 
    }

    int recibirPedidoRobot5(int tipo, MensajePedidoRobot5* buffer) {
        return this->recibir("ComunicacionRobot5MessageQueue", tipo, (void *)buffer, sizeof (MensajePedidoRobot5) - sizeof (long));
    }

    int enviarCanasto(MensajeRespuestaCanasto dato) {
        return this->enviar("ComunicacionRobot5MessageQueue", (const void *) &dato, sizeof (MensajeRespuestaCanasto) - sizeof (long)); 
    }

    int recibirCanasto(int tipo, MensajeRespuestaCanasto* buffer) {
        return this->recibir("ComunicacionRobot5MessageQueue", tipo, (void *)buffer, sizeof (MensajeRespuestaCanasto) - sizeof (long));
    }
    
    int enviarGabinete(MensajeRespuestaGabinete dato) {
        return this->enviar ("ComunicacionRobot5MessageQueue", (const void *) &dato, sizeof (MensajeRespuestaGabinete) - sizeof (long)); 
    }

    int recibirGabinete(int tipo, MensajeRespuestaGabinete* buffer) {
        return this->recibir("ComunicacionRobot5MessageQueue", tipo, (void *)buffer, sizeof (MensajeRespuestaGabinete) - sizeof (long));
    }
};

#endif // COMUNICACIONROBOT5_MESSAGEQUEUE_H_

