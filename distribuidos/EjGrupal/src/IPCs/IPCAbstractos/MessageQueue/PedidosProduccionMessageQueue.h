#ifndef PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_
#define PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

namespace IPC {
    
class PedidosProduccionMessageQueue : public AbstractMessageQueue
{

public:
	PedidosProduccionMessageQueue(std::string IPCName = ""):AbstractMessageQueue(IPCName) {} 
	
	virtual ~PedidosProduccionMessageQueue() {}

	int enviarPedidoProduccion (MensajePedidoProduccion dato) {
            return this->enviar((const void *)&dato,sizeof(MensajePedidoProduccion)-sizeof(long));
	}
	
	int recibirPedidoProduccion ( int tipo, MensajePedidoProduccion* buffer ) {
            return this->recibir(tipo, (void *)buffer, sizeof(MensajePedidoProduccion)-sizeof(long));
	}
        
        int enviarProximoPedidoProduccion (MensajeProximoPedidoProduccion dato) {
            return this->enviar((const void *)&dato,sizeof(MensajeProximoPedidoProduccion)-sizeof(long));
	}
	
	int recibirProximoPedidoProduccion ( int tipo, MensajeProximoPedidoProduccion* buffer ) {
            return this->recibir(tipo, (void *)buffer, sizeof(MensajeProximoPedidoProduccion)-sizeof(long));
	}

};

}

#endif // PEDIDOS_PRODUCCION_MESSAGEQUEUE_H_

