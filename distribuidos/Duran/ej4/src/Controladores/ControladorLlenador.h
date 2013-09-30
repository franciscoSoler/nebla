#ifndef CONTROLADOR_LLENADOR_H_
#define CONTROLADOR_LLENADOR_H_

#include "ControladorConsumidor.h"

class ControladorLlenador : public ControladorConsumidor {

public:

	ControladorLlenador(); 
	virtual ~ControladorLlenador();
	int obtenerRecipienteParaLlenar(Recipiente *recipiente);
};

#endif // CONTROLADOR_LLENADOR_H_
