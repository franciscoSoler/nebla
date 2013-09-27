#include "NumeradorSharedMemory.h"

//-----------------------------------------------------------------------------

size_t NumeradorSharedMemory::getMemorySize() {
	return sizeof(NumeradorInfo);
}

//-----------------------------------------------------------------------------
NumeradorInfo* NumeradorSharedMemory::readInfo () {
	return (NumeradorInfo*)(this->data);
}

//-----------------------------------------------------------------------------
void NumeradorSharedMemory::writeInfo ( NumeradorInfo *dato ) {
	NumeradorInfo *dir = (NumeradorInfo*)this->data;
	
	int i = 0;
	for (i = 0; i < MAX_CLIENTES; ++i) {
		(*dir).numerosClientes[i] = (*dato).numerosClientes[i];
	}
	for (i = 0; i < MAX_VENDEDORES; ++i) {
		(*dir).numerosVendedores[i] = (*dato).numerosVendedores[i];
	}
}

