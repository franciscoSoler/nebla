#ifndef ENTRADA_MESSAGEQUEUE_H_
#define ENTRADA_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../common.h"


class EntradaMessageQueue : public AbstractMessageQueue
{

public:
	EntradaMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~EntradaMessageQueue() {}

	int enviarMensajeEntrada (MensajeEntrada dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajeEntrada)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirMensajeEntrada ( int tipo, MensajeEntrada* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajeEntrada)-sizeof(long),tipo,0 );
		return resultado;
	}

};

#endif // ENTRADA_MESSAGEQUEUE_H_

