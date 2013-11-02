#ifndef PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_
#define PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../../Common.h"

namespace IPC {
    
class PedidosProduccionMessageQueue : public AbstractMessageQueue
{

public:
	PedidosProduccionMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~PedidosProduccionMessageQueue() {}

	int enviarPedidoProduccion (MensajePedidoProduccion dato) {
            return this->enviar((const void *)&dato,sizeof(MensajePedidoProduccion)-sizeof(long));
	}
	
	int recibirPedidoProduccion ( int tipo, MensajePedidoProduccion* buffer ) {
            return this->recibir(tipo, (void *)buffer, sizeof(MensajePedidoProduccion)-sizeof(long));
	}

};

}

#endif // PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

