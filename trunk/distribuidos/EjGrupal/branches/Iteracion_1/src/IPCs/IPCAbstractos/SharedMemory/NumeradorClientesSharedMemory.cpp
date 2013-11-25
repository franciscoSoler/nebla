#include "NumeradorClientesSharedMemory.h"


namespace IPC {

size_t NumeradorClientesSharedMemory::getMemorySize() {
	return sizeof(ClientesInfo);
}

ClientesInfo* NumeradorClientesSharedMemory::readInfo () {
	return (ClientesInfo*)(this->data);
}

void NumeradorClientesSharedMemory::writeInfo ( ClientesInfo *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}
