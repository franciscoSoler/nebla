#ifndef VENDER_MESSAGEQUEUE_H_
#define VENDER_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../common.h"

class VenderMessageQueue : public AbstractMessageQueue
{
public:
	
	VenderMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~VenderMessageQueue() {}

	int enviarNroVendedor (HandShakeMessage dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(HandShakeMessage)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirNroVendedor ( int tipo, HandShakeMessage* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(HandShakeMessage)-sizeof(long),tipo,0 );
		return resultado;
	}

	int enviarResultado (ResponseTicketsMessage dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(ResponseTicketsMessage)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirResultado ( int tipo, ResponseTicketsMessage* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(ResponseTicketsMessage)-sizeof(long),tipo,0 );
		return resultado;
	}

};

#endif


