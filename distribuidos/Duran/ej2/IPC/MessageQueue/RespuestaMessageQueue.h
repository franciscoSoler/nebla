#ifndef RESPUESTA_MESSAGEQUEUE_H_
#define RESPUESTA_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../../common.h"


class RespuestaMessageQueue : public AbstractMessageQueue
{

public:
	RespuestaMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~RespuestaMessageQueue() {}

	int enviarMensajeRespuesta (MensajeRespuesta dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajeRespuesta)-sizeof(long),0 );
		return resultado;
	}
	
	int recibirMensajeRespuesta ( int tipo, MensajeRespuesta* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajeRespuesta)-sizeof(long),tipo,0 );
		return resultado;
	}

};

#endif // RESPUESTA_MESSAGEQUEUE_H_


