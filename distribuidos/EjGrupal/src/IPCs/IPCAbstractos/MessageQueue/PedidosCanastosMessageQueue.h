/* 
 * File:   PedidosCanastosMessageQueue.h
 * Author: francisco
 *
 * Created on 25 de octubre de 2013, 0:13
 */

#ifndef PEDIDOSCANASTOSMESSAGEQUEUE_H
#define	PEDIDOSCANASTOSMESSAGEQUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../AbstractMessageQueue/AbstractMessageQueue.h"

namespace IPC {

class PedidosCanastosMessageQueue : public AbstractMessageQueue
{

public:
	PedidosCanastosMessageQueue(std::string IPCName = "") : AbstractMessageQueue(IPCName) {} 
	
	virtual ~PedidosCanastosMessageQueue() {}

	int enviarPedidoCanasto (MensajePedidoRobotCinta_6 dato) {
            int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajePedidoRobotCinta_6)-sizeof(long),0 );
            if (resultado == -1) {
                sprintf(this->buffer, "PedidosCanastosMessageQueue - Fallo la operacion send: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
            }
            return resultado;
	}
	
	int recibirPedidoCanasto ( int tipo, MensajePedidoRobotCinta_6* buffer ) {
            int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajePedidoRobotCinta_6)-sizeof(long),tipo,0 );
            if (resultado == -1) {
                sprintf(this->buffer, "PedidosCanastosMessageQueue - Fallo la operacion recv: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
            }
            return resultado;
	}

};

}

#endif	/* PEDIDOSCANASTOSMESSAGEQUEUE_H */

