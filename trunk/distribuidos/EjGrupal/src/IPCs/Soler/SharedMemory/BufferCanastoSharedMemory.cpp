#include "BufferCanastoSharedMemory.h"

namespace IPC {

//-----------------------------------------------------------------------------
size_t BufferCanastoSharedMemory::getMemorySize() {
	return sizeof(BufferCanastos);
}

//-----------------------------------------------------------------------------
void BufferCanastoSharedMemory::readInfo (BufferCanastos* dato) {
    memcpy(dato, this->data, getMemorySize());
}

//-----------------------------------------------------------------------------
void BufferCanastoSharedMemory::writeInfo ( BufferCanastos *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}