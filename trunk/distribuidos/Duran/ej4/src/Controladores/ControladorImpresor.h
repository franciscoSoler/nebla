#ifndef CONTROLADOR_IMPRESOR_H_
#define CONTROLADOR_IMPRESOR_H_

#include "ControladorConsumidor.h"

class ControladorImpresor : public ControladorConsumidor {

public:

	ControladorImpresor(); 
	virtual ~ControladorImpresor();
	int obtenerRecipienteParaImprimir(Recipiente *recipiente);
};

#endif // CONTROLADOR_IMPRESOR_H_
