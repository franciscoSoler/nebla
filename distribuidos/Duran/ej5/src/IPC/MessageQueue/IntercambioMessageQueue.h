#ifndef INTERCAMBIO_MESSAGEQUEUE_H_
#define INTERCAMBIO_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"
#include "../IPCException.h"

#include "../../common.h"


class IntercambioMessageQueue : public AbstractMessageQueue
{

public:
	IntercambioMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~IntercambioMessageQueue() {}

	int enviarMensajePersona (MensajePersona dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajePersona)-sizeof(long),0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion send: %s", strerror(errno));
			char className[255];
			strcpy(className, "IntercambioMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}
	
	int recibirMensajePersona ( int tipo, MensajePersona* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajePersona)-sizeof(long),tipo,0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion recv: %s", strerror(errno));
			char className[255];
			strcpy(className, "IntercambioMessageQueue");
			throw IPCException(className, error);

		}
		return resultado;
	}

};

#endif // INTERCAMBIO_MESSAGEQUEUE_H_

