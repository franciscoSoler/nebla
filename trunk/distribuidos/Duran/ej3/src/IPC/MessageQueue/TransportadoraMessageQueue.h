#ifndef TRANSPORTADORA_MESSAGEQUEUE_H_
#define TRANSPORTADORA_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"
#include "../IPCException.h"
#include "../../common.h"


class TransportadoraMessageQueue : public AbstractMessageQueue
{

public:
	TransportadoraMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~TransportadoraMessageQueue() {}

	int enviarMensajeAVerificar (MensajeVerificacionEquipaje dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajeVerificacionEquipaje)-sizeof(long),0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion send: %s", strerror(errno));
			char className[255];
			strcpy(className, "TransportadoraMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}
	
	int recibirMensajeAVerificar ( int tipo, MensajeVerificacionEquipaje* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajeVerificacionEquipaje)-sizeof(long),tipo,0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion recv: %s", strerror(errno));
			char className[255];
			strcpy(className, "TransportadoraMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}

};

#endif // TRANSPORTADORA_MESSAGEQUEUE_H_

