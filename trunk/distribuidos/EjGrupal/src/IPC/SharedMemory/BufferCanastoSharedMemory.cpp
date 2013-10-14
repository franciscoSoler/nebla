#include "BufferCanastoSharedMemory.h"

//-----------------------------------------------------------------------------
size_t BufferCanastoSharedMemory::getMemorySize() {
	return sizeof(Canasto);
}

//-----------------------------------------------------------------------------
Canasto* BufferCanastoSharedMemory::readInfo () {
	return (Canasto*)(this->data);
}

//-----------------------------------------------------------------------------
void BufferCanastoSharedMemory::writeInfo ( Canasto *dato ) {
    memcpy(this->data, dato, sizeof(Canasto));
}