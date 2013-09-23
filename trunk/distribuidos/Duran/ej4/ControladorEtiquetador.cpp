
#include "ControladorEtiquetador.h"
#include "common.h"

ControladorEtiquetador::ControladorEtiquetador() : ControladorConsumidor() {
}

ControladorEtiquetador::~ControladorEtiquetador () {
}

int ControladorEtiquetador::obtenerRecipienteParaEtiquetar(Recipiente *recipiente) {
	return obtenerRecipiente(ETIQUETADOR, recipiente);
}
