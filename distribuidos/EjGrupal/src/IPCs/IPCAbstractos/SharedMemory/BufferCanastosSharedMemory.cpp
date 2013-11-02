#include "BufferCanastosSharedMemory.h"

namespace IPC {

//-----------------------------------------------------------------------------
size_t BufferCanastosSharedMemory::getMemorySize() {
	return sizeof(BufferCanastos);
}

//-----------------------------------------------------------------------------
void BufferCanastosSharedMemory::readInfo (BufferCanastos* dato) {
    memcpy(dato, this->data, getMemorySize());
}

//-----------------------------------------------------------------------------
void BufferCanastosSharedMemory::writeInfo ( BufferCanastos *dato ) {
    memcpy(this->data, dato, getMemorySize());
}

}