#include "InfoVuelosSharedMemory.h"

//-----------------------------------------------------------------------------
size_t InfoVuelosSharedMemory::getMemorySize() {
	return sizeof(Informacion);
}

//-----------------------------------------------------------------------------
Informacion* InfoVuelosSharedMemory::readInfo () {
	return (Informacion*)(this->data);
}

//-----------------------------------------------------------------------------
void InfoVuelosSharedMemory::writeInfo ( Informacion *dato ) {
	
	Informacion *dir = (Informacion*)this->data;	
	
	for (int i = 0; i < TAM_BUFFER; ++i) {
		(*dir).vuelos[i].cantidad = (*dato).vuelos[i].cantidad;
		(*dir).vuelos[i].listo = (*dato).vuelos[i].listo;
	}
}

