
#include "ControladorLlenador.h"
#include "common.h"

ControladorLlenador::ControladorLlenador () : ControladorConsumidor() {
}

ControladorLlenador::~ControladorLlenador () {
}

int ControladorLlenador::obtenerRecipienteParaLlenar(Recipiente *recipiente) {
	return obtenerRecipiente(LLENADOR, recipiente);
}
