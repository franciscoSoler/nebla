/* 
 * File:   EspacioAlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 9, 2013, 10:40 PM
 */

#include "EspacioAlmacenPiezas.h"

#include "../../../Logger/Logger.h"

EspacioAlmacenPiezas::EspacioAlmacenPiezas() :
	espacioSharedMemory("Espacio Almacen de Piezas Sh Memory"),
	mutex("Mutex espacio almacen de Piezas"),
	bloqueoControladorAlmacenGabinetes("Bloqueo controlador almacen de Gabinetes"),
        bloqueoControladorAlmacenCanastos("Bloqueo controlador almacen de  Canastos")
{ 
}

void EspacioAlmacenPiezas::iniciarEspacioAlmacen()
{
    this->espacioSharedMemory.getSharedMemory(DIRECTORY_APIEZAS, LETRA_SHMEM_ALMACEN_PIEZAS);
    this->mutex.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ALMACEN_PIEZAS, 1);
    this->bloqueoControladorAlmacenGabinetes.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_GABINETES, 1);
    this->bloqueoControladorAlmacenCanastos.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_CANASTOS, 1);
}

Canasto EspacioAlmacenPiezas::obtenerCanastoConPiezas(TipoPieza numeroPieza)
{
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Buscando en el almacen el canasto %d.", numeroPieza);
    Logger::logMessage(Logger::TRACE, buffer);

    Canasto canasto;
    this->mutex.wait();
    {
	EstructuraAlmacenPiezas almacen;
	this->espacioSharedMemory.readInfo(&almacen);
	if(almacen.cantCanastos[numeroPieza - 1] < 1)
	{
	    sprintf(buffer, "No se encuentran canastos con piezas de tipo %d.", numeroPieza);
	    Logger::logMessage(Logger::IMPORTANT, buffer);
	    this->mutex.signal();
	    this->bloqueoControladorAlmacenCanastos.wait();

	    this->mutex.wait();
	}

	(almacen.cantCanastos[numeroPieza - 1])--;
	int cantCanastos = almacen.cantCanastos[numeroPieza - 1];
	canasto = almacen.canastos[numeroPieza - 1][cantCanastos];
	this->espacioSharedMemory.writeInfo(&almacen);
    }
    this->mutex.signal();
    return canasto;
}

Gabinete EspacioAlmacenPiezas::obtenerGabinete(TipoGabinete numeroGabinete)
{
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Buscando en el almacen algun gabinete de tipo %d.", numeroGabinete);
    Logger::logMessage(Logger::TRACE, buffer);

    Gabinete gabinete;
    this->mutex.wait();
    {
	EstructuraAlmacenPiezas almacen;
	this->espacioSharedMemory.readInfo(&almacen);
	if(almacen.cantGabinetes[numeroGabinete - 1] < 1)
	{
	    sprintf(buffer, "No se encuentran gabinetes de tipo %d.", numeroGabinete);
	    Logger::logMessage(Logger::IMPORTANT, buffer);
	    this->mutex.signal();
	    this->bloqueoControladorAlmacenGabinetes.wait();

	    this->mutex.wait();
	}

	(almacen.cantGabinetes[numeroGabinete - 1])--;
	int cantGabinetes = almacen.cantGabinetes[numeroGabinete - 1];
	gabinete = almacen.gabinetes[numeroGabinete - 1][cantGabinetes];
	this->espacioSharedMemory.writeInfo(&almacen);
    }
    this->mutex.signal();
    return gabinete;
}

EspacioAlmacenPiezas::EspacioAlmacenPiezas(const EspacioAlmacenPiezas& orig) { }

EspacioAlmacenPiezas::~EspacioAlmacenPiezas() { }

