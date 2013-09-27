#ifndef CONTROLADOR_CONSUMIDOR_H_
#define CONTROLADOR_CONSUMIDOR_H_

#include "./IPC/MessageQueue/RecipientesMessageQueue.h"
#include "common.h"

class ControladorConsumidor
{
private:
	
	RecipientesMessageQueue	recipientesQueue;
	
public:

	ControladorConsumidor(); 

	virtual ~ControladorConsumidor();

protected:

	int obtenerRecipiente(long mtype, Recipiente *recipiente);
};

#endif // CONTROLADOR_CONSUMIDOR_H_
