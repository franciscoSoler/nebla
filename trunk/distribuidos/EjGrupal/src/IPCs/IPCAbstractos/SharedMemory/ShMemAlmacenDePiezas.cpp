/* 
 * File:   ShMemAlmacenDePiezas.cpp
 * Author: fede
 * 
 * Created on November 9, 2013, 12:37 PM
 */

#include "ShMemAlmacenDePiezas.h"
#include "Semaphore.h"
#include "../../Logger/Logger.h"

#define MAX_ESPERA_REPONEDOR 20
#define MAX_ESPERA_ENCONTRAR_PIEZA 5

namespace IPC {
    
    ShMemAlmacenDePiezas::ShMemAlmacenDePiezas(std::string IPCName) : AbstractSharedMemory(IPCName)
    {
	sprintf(buffer, "Almacén de piezas físico");
	Logger::setProcessInformation(buffer);
	this->mutex = IPC::Semaphore("mutexAlmacenDePiezas");
    }
    
    ShMemAlmacenDePiezas::~ShMemAlmacenDePiezas() { }

    size_t ShMemAlmacenDePiezas::getMemorySize()
    {
	return CANTIDAD_TIPOS_PIEZAS * sizeof(int);    
    }
    
    void ShMemAlmacenDePiezas::reponerPiezas(int numeroPieza)
    {
	sprintf(buffer, "Sin stock de pieza %d, esperando que se repongan.", numeroPieza);
	Logger::logMessage(Logger::TRACE, buffer);
	
	int demoraReposicion = rand() % MAX_ESPERA_REPONEDOR + 1;
	usleep(demoraReposicion * 1000 * 1000);
	
	int cantidadReposicion = CANTIDAD_MINIMA_REPOSICION_PIEZAS + 
		(rand() % (CANTIDAD_MAXIMA_REPOSICION_PIEZAS - CANTIDAD_MINIMA_REPOSICION_PIEZAS));
	this->almacen[numeroPieza - 1] += cantidadReposicion;
    }
    
    Canasto ShMemAlmacenDePiezas::obtenerCanastoConPiezas(int numeroPieza)
    {
	sprintf(buffer, "Buscando en el almacén pieza %d.", numeroPieza);
	Logger::setProcessInformation(buffer);
	
	int demoraBusqueda = rand() % MAX_ESPERA_ENCONTRAR_PIEZA + 1;
	usleep(demoraBusqueda * 1000 * 1000);
	
	Canasto canasto;
	int cantidadPiezas = rand() % MAX_QUANTITY_CANASTOS;
		
	this->mutex.wait();
	{
	    if(this->almacen[numeroPieza - 1] < cantidadPiezas)
		reponerPiezas(numeroPieza);
	    this->almacen[numeroPieza - 1] -= cantidadPiezas;
	}
	this->mutex.signal();
	
	canasto.cantidadPiezas = cantidadPiezas;
	canasto.tipoPieza = static_cast<TipoPieza>(numeroPieza);
	return canasto;
    }

}