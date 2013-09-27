#include "TicketsInfoSharedMemory.h"

//-----------------------------------------------------------------------------

size_t TicketsInfoSharedMemory::getMemorySize() {
	return sizeof(TicketsInfo);
}

//-----------------------------------------------------------------------------
TicketsInfo* TicketsInfoSharedMemory::readInfo () {
	return (TicketsInfo*)(this->data);
}

//-----------------------------------------------------------------------------
void TicketsInfoSharedMemory::writeInfo ( TicketsInfo *dato ) {
	TicketsInfo *dir = (TicketsInfo*)this->data;
	(*dir).cantidad = (*dato).cantidad;
	(*dir).dinero = (*dato).dinero;
}

