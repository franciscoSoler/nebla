#ifndef PEDIDOSAGV_MESSAGEQUEUE_H_
#define PEDIDOSAGV_MESSAGEQUEUE_H_

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
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(PedidoAgv)-sizeof(long),0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion send: %s", strerror(errno));
			char className[255];
			strcpy(className, "PedidosAgvMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}
	
	int recibirPedidoAgv ( int tipo, PedidoAgv* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(PedidoAgv)-sizeof(long),tipo,0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion recv: %s", strerror(errno));
			char className[255];
			strcpy(className, "PedidosAgvMessageQueue");
			throw IPCException(className, error);

		}
		return resultado;
	}

};

#endif // PEDIDOSAGV_MESSAGEQUEUE_H_

