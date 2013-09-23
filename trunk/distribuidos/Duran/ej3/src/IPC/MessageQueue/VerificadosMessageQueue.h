#ifndef VERIFICADOS_MESSAGEQUEUE_H_
#define VERIFICADOS_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"
#include "../IPCException.h"
#include "../../common.h"


class VerificadosMessageQueue : public AbstractMessageQueue
{

public:
	VerificadosMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~VerificadosMessageQueue() {}

	int enviarMensajeVerificado (MensajeIntercambioEquipaje dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajeIntercambioEquipaje)-sizeof(long),0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion send: %s", strerror(errno));
			char className[255];
			strcpy(className, "VerificadosMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}
	
	int recibirMensajeVerificado ( int tipo, MensajeIntercambioEquipaje* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajeIntercambioEquipaje)-sizeof(long),tipo,0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion recv: %s", strerror(errno));
			char className[255];
			strcpy(className, "VerificadosMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}

};

#endif // VERIFICADOS_MESSAGEQUEUE_H_


