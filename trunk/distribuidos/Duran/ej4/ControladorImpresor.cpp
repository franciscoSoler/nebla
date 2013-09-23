
#include "ControladorImpresor.h"
#include "common.h"

ControladorImpresor::ControladorImpresor() : ControladorConsumidor() {
}

ControladorImpresor::~ControladorImpresor () {
}

int ControladorImpresor::obtenerRecipienteParaImprimir(Recipiente *recipiente) {
	return obtenerRecipiente(IMPRESOR, recipiente);
}
