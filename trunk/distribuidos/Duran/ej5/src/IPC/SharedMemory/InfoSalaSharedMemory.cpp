#include "InfoSalaSharedMemory.h"

//-----------------------------------------------------------------------------
size_t InfoSalaSharedMemory::getMemorySize() {
	return sizeof(InfoSala);
}

//-----------------------------------------------------------------------------
InfoSala* InfoSalaSharedMemory::readInfo () {
	return (InfoSala*)(this->data);
}

//-----------------------------------------------------------------------------
void InfoSalaSharedMemory::writeInfo ( InfoSala *dato ) {
	InfoSala *dir = (InfoSala*)this->data;
	
	(*dir).puertaBloqueada = (*dato).puertaBloqueada;
	
	for (int i = 0; i<TAM_SALA; ++i) {
		(*dir).idPersonas[i] = (*dato).idPersonas[i];
	}

	for (int i = 0; i<MAX_BUSES; ++i) { 
		(*dir).busBloqueado[i] = (*dato).busBloqueado[i];
	}
	
	(*dir).busEnSala = (*dato).busEnSala;
}

