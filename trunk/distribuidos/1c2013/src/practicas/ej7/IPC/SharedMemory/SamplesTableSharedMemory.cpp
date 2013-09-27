#include "SamplesTableSharedMemory.h"

//-----------------------------------------------------------------------------

size_t SamplesTableSharedMemory::getMemorySize() {
	return sizeof(SamplesTable);
}

//-----------------------------------------------------------------------------
SamplesTable* SamplesTableSharedMemory::readInfo () {
	return (SamplesTable*)(this->data);
}

//-----------------------------------------------------------------------------
void SamplesTableSharedMemory::writeInfo ( SamplesTable *dato ) {
	SamplesTable *dir = (SamplesTable*)this->data;
	(*dir) = (*dato);
}
