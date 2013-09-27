#include "ControlDataSharedMemory.h"

size_t ControlDataSharedMemory::getMemorySize()
{
	// Son todos enteros, no tengo problema en realizar un sizeof de 
	// la estructura
	return sizeof(ControlData);
}

void ControlDataSharedMemory::read(ControlData* aControlData) const
{
	ControlData* aux = (ControlData*) this->data;
	aControlData->cantPersonas = aux->cantPersonas;
}

void ControlDataSharedMemory::write(const ControlData* aControlData)
{
	ControlData* aux = (ControlData*) this->data;
	aux->cantPersonas = aControlData->cantPersonas;
}

