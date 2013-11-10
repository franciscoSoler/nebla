/* 
 * File:   ControladorRepositor.cpp
 * Author: fede
 * 
 * Created on November 9, 2013, 5:30 PM
 */

#include "ControladorRepositor.h"

ControladorRepositor::ControladorRepositor() :
	almacenSharedMemory("almacenPiezasShMemory"),
	bloqueoControladorGabinetes("Espera Repositor Gabinete"),
	bloqueoControladorCanastos("Espera Repositor Canastos"),
	mutex("mutexAlmacenDePiezas")
{ 
}

ControladorRepositor::~ControladorRepositor() { }

void ControladorRepositor::iniciarControlador () {
    this->almacenSharedMemory.getSharedMemory(DIRECTORY_APIEZAS, LETRA_SHMEM_ALMACEN_PIEZAS);
    this->mutex.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ALMACEN_PIEZAS, 1);
    this->bloqueoControladorGabinetes.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_GABINETES, 1);
    this->bloqueoControladorCanastos.getSemaphore(DIRECTORY_APIEZAS, LETRA_SEM_ESPERA_REPOSITOR_CANASTOS, 1);
}

void ControladorRepositor::reponerCanastos(int numeroPieza)
{
    mutex.wait();
    {
	int cantCanastosAReponer = CANTIDAD_MINIMA_ITEMS_REPOSICION + 
		    (rand() % (CANTIDAD_MAXIMA_ITEMS_REPOSICION - CANTIDAD_MINIMA_ITEMS_REPOSICION));

	EstructuraAlmacenPiezas estructuraAlmacen;
	this->almacenSharedMemory.readInfo(&estructuraAlmacen);
	for(int i = 0; i < cantCanastosAReponer; i++)
	{
	    
	    int cantidadPiezasReposicion = CANTIDAD_MINIMA_REPOSICION_PIEZAS + 
		    (rand() % (CANTIDAD_MAXIMA_REPOSICION_PIEZAS - CANTIDAD_MINIMA_REPOSICION_PIEZAS));
	    Canasto canasto;
	    canasto.cantidadPiezas = cantidadPiezasReposicion;
	    canasto.tipoPieza = static_cast<TipoPieza>(numeroPieza);
	    
	    int cantCanastos = estructuraAlmacen.cantCanastos[numeroPieza - 1];
	    estructuraAlmacen.canastos[numeroPieza - 1][cantCanastos] = canasto;
	    (estructuraAlmacen.cantCanastos[numeroPieza - 1])++;
	}
	this->almacenSharedMemory.writeInfo(&estructuraAlmacen);
        this->bloqueoControladorCanastos.signal();
    }
    mutex.signal();
}

void ControladorRepositor::reponerGabinetes(int numeroGabinete)
{
    mutex.wait();
    {
	int cantGabinetesAReponer = CANTIDAD_MINIMA_ITEMS_REPOSICION + 
		    (rand() % (CANTIDAD_MAXIMA_ITEMS_REPOSICION - CANTIDAD_MINIMA_ITEMS_REPOSICION));
	
	EstructuraAlmacenPiezas estructuraAlmacen;
	this->almacenSharedMemory.readInfo(&estructuraAlmacen);
	for(int i = 0; i < cantGabinetesAReponer; i++)
	{
	    Gabinete gabinete;
	    gabinete.tipoGabinete = static_cast<TipoGabinete>(numeroGabinete);
	    	
	    int cantGabinetes = estructuraAlmacen.cantGabinetes[numeroGabinete - 1];
	    estructuraAlmacen.gabinetes[numeroGabinete - 1][cantGabinetes] = gabinete;
	    (estructuraAlmacen.cantGabinetes[cantGabinetes - 1])++;	    
	}
	this->almacenSharedMemory.writeInfo(&estructuraAlmacen);
        this->bloqueoControladorGabinetes.signal();

    }
    mutex.signal();
}