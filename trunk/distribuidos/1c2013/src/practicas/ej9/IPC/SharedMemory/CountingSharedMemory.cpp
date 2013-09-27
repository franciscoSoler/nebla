#include "CountingSharedMemory.h"

//-----------------------------------------------------------------------------

size_t CountingSharedMemory::getMemorySize() {
	return sizeof(int);
}

//-----------------------------------------------------------------------------
int* CountingSharedMemory::readInfo () {
	return (int*)(this->data);
}

//-----------------------------------------------------------------------------
void CountingSharedMemory::writeInfo ( int *dato ) {
	int *dir = (int*)this->data;
	(*dir) = (*dato);
}

