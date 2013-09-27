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
	aControlData->estado = aux->estado;
	aControlData->cLectoresLeyendo = aux->cLectoresLeyendo;
	aControlData->cLectoresEsperando = aux->cLectoresEsperando;
	aControlData->cEscritoresEscribiendo =  aux->cEscritoresEscribiendo;
	aControlData->cEscritoresEsperando = aux->cEscritoresEsperando;
	aControlData->cAccesos = aux->cAccesos;
}

void ControlDataSharedMemory::write(const ControlData* aControlData)
{
	ControlData* aux = (ControlData*) this->data;
	aux->estado = aControlData->estado;
	aux->cLectoresLeyendo = aControlData->cLectoresLeyendo;
	aux->cLectoresEsperando = aControlData->cLectoresEsperando;
	aux->cEscritoresEscribiendo = aControlData->cEscritoresEscribiendo;
	aux->cEscritoresEsperando = aControlData->cEscritoresEsperando;
	aux->cAccesos = aControlData->cAccesos;
}

