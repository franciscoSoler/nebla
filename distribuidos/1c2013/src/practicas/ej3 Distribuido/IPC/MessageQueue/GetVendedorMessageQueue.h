#ifndef GETVENDEDOR_MESSAGEQUEUE_H_
#define GETVENDEDOR_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../common.h"


class GetVendedorMessageQueue : public AbstractMessageQueue
{
public:
	GetVendedorMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~GetVendedorMessageQueue() {}

	int enviarPedido (HandShakeMessage dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(HandShakeMessage)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirPedido ( int tipo,HandShakeMessage* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(HandShakeMessage)-sizeof(long),tipo,0 );
		return resultado;
	}

};

#endif


