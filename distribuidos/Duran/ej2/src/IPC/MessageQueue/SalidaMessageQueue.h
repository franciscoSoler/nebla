#ifndef SALIDA_MESSAGEQUEUE_H_
#define SALIDA_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../common.h"


class SalidaMessageQueue : public AbstractMessageQueue
{

public:
	SalidaMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~SalidaMessageQueue() {}

	int enviarMensajeSalida (MensajeSalida dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajeSalida)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirMensajeSalida ( int tipo, MensajeSalida* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajeSalida)-sizeof(long),tipo,0 );
		return resultado;
	}

};

#endif // SALIDA_MESSAGEQUEUE_H_


