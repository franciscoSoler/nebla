
#include "ControladorPersona.h"
#include "./IPC/IPCException.h"
#include "common.h"

ControladorPersona::ControladorPersona (int nroPersona) {
	this->nroPersona = nroPersona;
	this->intercambio = IntercambioMessageQueue();
	try {
		// Obteniendo la cola de mensajes
		this->intercambio.getMessageQueue((char*)DIRECTORY, ID_COLA_INTERCAMBIO);
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Controlador Persona %d: %s\n", nroPersona, ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
}

ControladorPersona::~ControladorPersona () {
}

void ControladorPersona::enviarEquipaje(Equipaje equipaje, int vuelo) {
	
	MensajeIntercambioEquipaje mensajeIntercambio;
	mensajeIntercambio.mtype = vuelo;
	mensajeIntercambio.nroPersona = nroPersona;
	mensajeIntercambio.equipaje = equipaje;
	
	try {
		this->intercambio.enviarMensajeConEquipaje(mensajeIntercambio);
	}
	catch (IPCException &ex) {
		char buffer[TAM_BUFFER];
		sprintf (buffer, "Controlador Persona %d: %s\n", nroPersona, ex.getDescription());
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}

}
