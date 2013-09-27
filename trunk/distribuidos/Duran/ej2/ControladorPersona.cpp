
#include "ControladorPersona.h"
#include "common.h"

ControladorPersona::ControladorPersona (int nroPersona) {
	
	idPersona = nroPersona;
	
	// Obteniendo las colas de mensajes
	entradaMessageQueue = EntradaMessageQueue();
	entradaMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_ENTRADA);
	
	salidaMessageQueue = SalidaMessageQueue();
	salidaMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_SALIDA);
	
	respuestaMessageQueue = RespuestaMessageQueue();
	respuestaMessageQueue.getMessageQueue((char*)DIRECTORY, ID_COLA_RESPUESTA);
}

ControladorPersona::~ControladorPersona () {
}

bool ControladorPersona::entrar(int nroPuerta) {
	
	MensajeEntrada entrada;
	entrada.mtype = nroPuerta;
	entrada.idPersona = idPersona;
	
	entradaMessageQueue.enviarMensajeEntrada(entrada);
	
	MensajeRespuesta respuesta;
	respuestaMessageQueue.recibirMensajeRespuesta(idPersona, &respuesta);
	
	return respuesta.resultado;
}

void ControladorPersona::salir(int nroPuerta) {
	MensajeSalida salida;
	salida.mtype = nroPuerta;
	salida.idPersona = idPersona;
	
	salidaMessageQueue.enviarMensajeSalida(salida);
}
