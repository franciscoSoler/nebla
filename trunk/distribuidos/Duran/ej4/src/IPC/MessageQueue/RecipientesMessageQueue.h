#ifndef COMPRAR_MESSAGEQUEUE_H_
#define COMPRAR_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../common.h"


class RecipientesMessageQueue : public AbstractMessageQueue
{

public:
	RecipientesMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~RecipientesMessageQueue() {}

	int enviarRecipiente (RecipienteMessage dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(RecipienteMessage)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirRecipiente ( int tipo, RecipienteMessage* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(RecipienteMessage)-sizeof(long),tipo,0 );
		return resultado;
	}

};

#endif


