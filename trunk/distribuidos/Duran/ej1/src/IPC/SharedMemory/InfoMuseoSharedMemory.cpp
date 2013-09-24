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
	(*dir).total = (*dato).total;
	(*dir).cantidadMaxima = (*dato).cantidadMaxima;
}

