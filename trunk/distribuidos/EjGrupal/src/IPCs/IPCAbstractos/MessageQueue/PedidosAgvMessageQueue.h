#ifndef PEDIDOS_AGV_MESSAGEQUEUE_H_
#define PEDIDOS_AGV_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {

class PedidosAgvMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAgvMessageQueue(std::string IPCName = ""):AbstractMessageQueue(IPCName) {} 
	
	virtual ~PedidosAgvMessageQueue() {}

	int enviarPedidoAgv (MensajePedidoAgv dato) {
            return this->enviar((const void *)&dato,sizeof(MensajePedidoAgv)-sizeof(long));
	}
	
	int recibirPedidoAgv ( int tipo, MensajePedidoAgv* buffer ) {
            return this->recibir(tipo, (void *)buffer, sizeof(MensajePedidoAgv)-sizeof(long));
	}

};

}

#endif // PEDIDOS_AGV_MESSAGEQUEUE_H_

