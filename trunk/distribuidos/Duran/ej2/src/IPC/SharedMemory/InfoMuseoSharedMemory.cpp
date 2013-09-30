#include "InfoMuseoSharedMemory.h"

//-----------------------------------------------------------------------------
size_t InfoMuseoSharedMemory::getMemorySize() {
	return sizeof(InfoMuseo);
}

//-----------------------------------------------------------------------------
InfoMuseo* InfoMuseoSharedMemory::readInfo () {
	return (InfoMuseo*)(this->data);
}

//-----------------------------------------------------------------------------
void InfoMuseoSharedMemory::writeInfo ( InfoMuseo *dato ) {
	InfoMuseo *dir = (InfoMuseo*)this->data;
	(*dir).abierto = (*dato).abierto;
	(*dir).cantidad = (*dato).cantidad;
	(*dir).cantidadMaxima = (*dato).cantidadMaxima;
	
	for (int i = 0; i < TAM_BUFFER; ++i) {
		(*dir).puertasBloqueadas[i] = (*dato).puertasBloqueadas[i];
	}
}

