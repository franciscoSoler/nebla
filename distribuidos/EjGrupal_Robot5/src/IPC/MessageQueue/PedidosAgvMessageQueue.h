#ifndef PEDIDOS_AGV_MESSAGEQUEUE_H_
#define PEDIDOS_AGV_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"
#include "../IPCException.h"

#include "../../common.h"


class PedidosAgvMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAgvMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~PedidosAgvMessageQueue() {}

	int enviarPedidoAgv (PedidoAgv dato) {
            return this->enviar("PedidosAgvMessageQueue", (const void *)&dato,sizeof(PedidoAgv)-sizeof(long));
	}
	
	int recibirPedidoAgv ( int tipo, PedidoAgv* buffer ) {
            return this->recibir("PedidosAgvMessageQueue", tipo, (void *)buffer, sizeof(PedidoAgv)-sizeof(long));
	}

};

#endif // PEDIDOS_AGV_MESSAGEQUEUE_H_

