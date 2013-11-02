#ifndef PEDIDOS_AGV_MESSAGEQUEUE_H_
#define PEDIDOS_AGV_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {

class PedidosAgvMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAgvMessageQueue(std::string IPCName = ""):AbstractMessageQueue(IPCName) {} 
	
	virtual ~PedidosAgvMessageQueue() {}

	int enviarPedidoAgv (MensajePedidoAgv_5 dato) {
            return this->enviar((const void *)&dato,sizeof(MensajePedidoAgv_5)-sizeof(long));
	}
	
	int recibirPedidoAgv ( int tipo, MensajePedidoAgv_5* buffer ) {
            return this->recibir(tipo, (void *)buffer, sizeof(MensajePedidoAgv_5)-sizeof(long));
	}

};

}

#endif // PEDIDOS_AGV_MESSAGEQUEUE_H_

