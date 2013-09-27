#include "MuestrasSharedMemory.h"

//-----------------------------------------------------------------------------
size_t MuestrasSharedMemory::getMemorySize() {
	return sizeof(Muestras);
}

//-----------------------------------------------------------------------------
Muestras* MuestrasSharedMemory::readInfo () {
	return (Muestras*)(this->data);
}

//-----------------------------------------------------------------------------
void MuestrasSharedMemory::writeInfo ( Muestras *dato ) {
	Muestras *dir = (Muestras*)this->data;

	for (int i = 0; i < CANT_MUESTRAS; ++i) {
		(*dir).muestra[i] = (*dato).muestra[i];
		(*dir).productorBloqueado[i] = (*dato).productorBloqueado[i];
	}

	for (int i = 0; i < CANT_ANALIZADORES; ++i) {
		(*dir).analizadorBloqueado[i] = (*dato).analizadorBloqueado[i];
	}

	for (int j = 0; j < CANT_ANALIZADORES; ++j) {
		for (int i = 0; i < CANT_MUESTRAS; ++i) {
			(*dir).estadoMuestras[j][i] = (*dato).estadoMuestras[j][i];
		}
	}
}
