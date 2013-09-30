#ifndef CONTROLADOR_ETIQUETADOR_H_
#define CONTROLADOR_ETIQUETADOR_H_

#include "ControladorConsumidor.h"

class ControladorEtiquetador : public ControladorConsumidor {

public:

	ControladorEtiquetador(); 
	virtual ~ControladorEtiquetador();
	int obtenerRecipienteParaEtiquetar(Recipiente *recipiente);
};

#endif // CONTROLADOR_ETIQUETADOR_H_
