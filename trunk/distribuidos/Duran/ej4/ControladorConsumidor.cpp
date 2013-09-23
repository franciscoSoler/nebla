
#include "ControladorConsumidor.h"
#include "common.h"

ControladorConsumidor::ControladorConsumidor () {
	
	// Obteniendo la cola de mensajes
	recipientesQueue = RecipientesMessageQueue();
	recipientesQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_RECIPIENTES);
}

ControladorConsumidor::~ControladorConsumidor () {
}

int ControladorConsumidor::obtenerRecipiente(long mtype, Recipiente *recipiente) {
	
	RecipienteMessage recipienteMessage;
	
	int resultado = recipientesQueue.recibirRecipiente(mtype,&recipienteMessage);
	if (resultado < 0) return resultado;
	
	(*recipiente) = recipienteMessage.recipiente;
	return 0;
}
