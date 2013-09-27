
#include "ControladorProductor.h"

#include <iostream>
#include <errno.h>

#include "common.h"

ControladorProductor::ControladorProductor() {
	// Obtener la cola de mensajes
	recipientesQueue = RecipientesMessageQueue();
	recipientesQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_RECIPIENTES);
}

ControladorProductor::~ControladorProductor () {
}

bool ControladorProductor::depositarRecipiente(Recipiente recipiente) {
	
	char buffer[TAM_BUFFER];
	
	RecipienteMessage recipienteLlenar;
	recipienteLlenar.mtype = LLENADOR;
	recipienteLlenar.recipiente = recipiente;

	RecipienteMessage recipienteEtiquetar;
	recipienteEtiquetar.mtype = ETIQUETADOR;
	recipienteEtiquetar.recipiente = recipiente;
	
	RecipienteMessage recipienteImprimir;
	recipienteImprimir.mtype = IMPRESOR;
	recipienteImprimir.recipiente = recipiente;
	
	if (recipientesQueue.enviarRecipiente(recipienteLlenar) == -1) {
		sprintf(buffer, "Controlador productor: Error al enviar el recipiente a llenar: %d", errno);
		write(fileno(stderr), buffer, strlen(buffer));
		return false;
	}

	if (recipientesQueue.enviarRecipiente(recipienteEtiquetar) == -1) {
		sprintf(buffer, "Controlador productor: Error al enviar el recipiente a etiquetar: %d", errno);
		write(fileno(stderr), buffer, strlen(buffer));
		return false;
	}
	
	if (recipientesQueue.enviarRecipiente(recipienteImprimir) == -1) {
		sprintf(buffer, "Controlador productor: Error al enviar el recipiente a imprimir: %d", errno);
		write(fileno(stderr), buffer, strlen(buffer));
		return false;
	}
	
	return true;
	
}
