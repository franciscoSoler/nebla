/* 
 * File:   PedidosAGVMessageQueue.h
 * Author: francisco
 *
 * Created on 26 de octubre de 2013, 20:59
 */

#ifndef PEDIDOSAGVMESSAGEQUEUE_H
#define	PEDIDOSAGVMESSAGEQUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

namespace IPC {

class PedidosAGVMessageQueue : public AbstractMessageQueue
{

public:
	PedidosAGVMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~PedidosAGVMessageQueue() {}

	int enviarPedidoAGV (PedidoCanastoAGV dato) {
            int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(PedidoCanastoAGV)-sizeof(long),0 );
            if (resultado == -1) {
                sprintf(this->buffer, "PedidosAGVMessageQueue - Fallo la operacion send: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
            }
            return resultado;
	}
	
	int recibirPedidoAGV ( int tipo, PedidoCanastoAGV* buffer ) {
            int resultado = msgrcv ( this->id,(void *)buffer,sizeof(PedidoCanastoAGV)-sizeof(long),tipo,0 );
            if (resultado == -1) {
                sprintf(this->buffer, "PedidosAGVMessageQueue - Fallo la operacion recv: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
            }
            return resultado;
	}
};

}

#endif	/* PEDIDOSAGVMESSAGEQUEUE_H */

