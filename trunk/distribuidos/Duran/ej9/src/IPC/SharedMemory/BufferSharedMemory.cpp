#include "BufferSharedMemory.h"

//-----------------------------------------------------------------------------
size_t BufferSharedMemory::getMemorySize() {
	return sizeof(Buffer);
}

//-----------------------------------------------------------------------------
Buffer* BufferSharedMemory::readInfo () {
	return (Buffer*)(this->data);
}

//-----------------------------------------------------------------------------
void BufferSharedMemory::writeInfo ( Buffer *dato ) {
	Buffer *dir = (Buffer*)this->data;

	(*dir).contador = (*dato).contador;
	for (int i = 0; i < MAX_PROCESADORES; ++i) {
		(*dir).informacion[i] = (*dato).informacion[i];
	}
}

