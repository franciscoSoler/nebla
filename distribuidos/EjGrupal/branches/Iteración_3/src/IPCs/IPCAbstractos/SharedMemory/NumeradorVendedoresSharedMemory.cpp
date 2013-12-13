#include "NumeradorVendedoresSharedMemory.h"


namespace IPC {

size_t NumeradorVendedoresSharedMemory::getMemorySize() {
	return sizeof(VendedoresInfo);
}

VendedoresInfo* NumeradorVendedoresSharedMemory::readInfo () {
	return (VendedoresInfo*)(this->data);
}

void NumeradorVendedoresSharedMemory::writeInfo ( VendedoresInfo *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}
