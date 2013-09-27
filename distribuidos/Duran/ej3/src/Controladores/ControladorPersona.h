#ifndef CONTROLADOR_PERSONA_H_
#define CONTROLADOR_PERSONA_H_

#include "./IPC/MessageQueue/IntercambioMessageQueue.h"
#include "common.h"

class ControladorPersona {
private:
	IntercambioMessageQueue intercambio;
	int nroPersona;
	
public:
	ControladorPersona(int nroPersona);
	virtual ~ControladorPersona();

	void enviarEquipaje(Equipaje equipaje, int vuelo);
};

#endif // CONTROLADOR_PERSONA_H_

