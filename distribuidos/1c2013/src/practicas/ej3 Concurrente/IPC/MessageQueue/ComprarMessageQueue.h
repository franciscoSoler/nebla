#ifndef COMPRAR_MESSAGEQUEUE_H_
#define COMPRAR_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../common.h"


class ComprarMessageQueue : public AbstractMessageQueue
{

public:
	ComprarMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~ComprarMessageQueue() {}

	int enviarPedido (RequestTicketsMessage dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(RequestTicketsMessage)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirPedido ( int tipo, RequestTicketsMessage* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(RequestTicketsMessage)-sizeof(long),tipo,0 );
		return resultado;
	}

};

#endif


