#ifndef PELIGROSOS_MESSAGEQUEUE_H_
#define PELIGROSOS_MESSAGEQUEUE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

#include "../IPCException.h"
#include "../../common.h"


class PeligrososMessageQueue : public AbstractMessageQueue
{

public:
	PeligrososMessageQueue():AbstractMessageQueue() {} 
	
	virtual ~PeligrososMessageQueue() {}

	int enviarMensajeConEquipajePeligroso (MensajeEquipajeSospechoso dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MensajeEquipajeSospechoso)-sizeof(long),0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion send: %s", strerror(errno));
			char className[255];
			strcpy(className, "PeligrososoMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}
	
	int recibirMensajeConEquipajePeligroso ( int tipo, MensajeEquipajeSospechoso* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MensajeEquipajeSospechoso)-sizeof(long),tipo,0 );
		if (resultado == -1) {
			char error[255];
			sprintf(error, "Fallo la operacion recv: %s", strerror(errno));
			char className[255];
			strcpy(className, "PeligrososMessageQueue");
			throw IPCException(className, error);
		}
		return resultado;
	}

};

#endif // PELIGROSOS_MESSAGEQUEUE_H_


