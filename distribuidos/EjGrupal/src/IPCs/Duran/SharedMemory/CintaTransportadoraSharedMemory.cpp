#include "CintaTransportadoraSharedMemory.h"

//-----------------------------------------------------------------------------
size_t CintaTransportadoraSharedMemory::getMemorySize() {
	return sizeof(CintaTransportadora_6);
}

//-----------------------------------------------------------------------------
CintaTransportadora_6* CintaTransportadoraSharedMemory::readInfo () {
	return (CintaTransportadora_6*)(this->data);
}

//-----------------------------------------------------------------------------
void CintaTransportadoraSharedMemory::writeInfo (CintaTransportadora_6 *dato) {
    memcpy(this->data, dato, sizeof(CintaTransportadora_6));
}