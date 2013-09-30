#ifndef CONTROLADOR_PRODUCTOR_H_
#define CONTROLADOR_PRODUCTOR_H_

#include "./IPC/MessageQueue/RecipientesMessageQueue.h"
#include "common.h"

class ControladorProductor
{
private:
	
	int nroProductor;
	RecipientesMessageQueue	recipientesQueue;
	
public:

	ControladorProductor(); 

	virtual ~ControladorProductor();

	bool depositarRecipiente(Recipiente recipiente);
};

#endif // CONTROLADOR_CLIENTE_H_
