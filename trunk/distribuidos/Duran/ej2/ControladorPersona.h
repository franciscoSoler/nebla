#ifndef CONTROLADOR_PERSONA_H_
#define CONTROLADOR_PERSONA_H_

#include "./IPC/MessageQueue/EntradaMessageQueue.h"
#include "./IPC/MessageQueue/SalidaMessageQueue.h"
#include "./IPC/MessageQueue/RespuestaMessageQueue.h"

class ControladorPersona
{
	
private:

	int idPersona;

	EntradaMessageQueue entradaMessageQueue;
	SalidaMessageQueue salidaMessageQueue;
	RespuestaMessageQueue respuestaMessageQueue;
	
public:
	ControladorPersona(int nroPersona);
	virtual ~ControladorPersona();

	bool entrar(int nroPuerta);
	void salir(int nroPuerta);
};

#endif // CONTROLADOR_PERSONA_H_

