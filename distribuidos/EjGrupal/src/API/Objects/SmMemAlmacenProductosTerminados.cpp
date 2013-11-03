/* 
 * File:   SmMemAlmacenProductosTerminados.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 10:34 PM
 */

#include "SmMemAlmacenProductosTerminados.h"

SmMemAlmacenProductosTerminados::SmMemAlmacenProductosTerminados() 
{
    this->shmemAlmacenTerminados = MemoriaCompartida(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, TAM_ALMACEN * sizeof(EspacioAlmacenProductos));
    this->almacenTerminados = (EspacioAlmacenProductos*) shmemAlmacenTerminados.obtener();
}

SmMemAlmacenProductosTerminados::~SmMemAlmacenProductosTerminados() 
{
    
    
}

int SmMemAlmacenProductosTerminados::obtenerEspaciosVacios()
{
    int cantidadEspacioVacio = 0;
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO)
	    cantidadEspacioVacio++;
    }
    return cantidadEspacioVacio;
}

int SmMemAlmacenProductosTerminados::obtenerCantidadDeStockDeProducto(int numProducto)
{
    int cantidadEnStock = 0;
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == RESERVADO_DISPONIBLE ||
		almacenTerminados[numeroEspacio].estado == PRODUCTO_DISPONIBLE)
	{
	    if(almacenTerminados[numeroEspacio].ordenProduccion.tipoProducto == numProducto)
		cantidadEnStock++;
	}
    }
    
    return cantidadEnStock;
}

bool SmMemAlmacenProductosTerminados::depositarCaja(Caja caja)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].ordenCompra.numero == caja.ordenDeCompra_)
	{
	    almacenTerminados[numeroEspacio].caja = caja;
	    return true;
	}
    }
    return false;
}

bool SmMemAlmacenProductosTerminados::sacarCaja(Caja* caja, int idProducto, int idOrdenCompra)
{
    bool primeraCajaEncontrada = false;
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].ordenCompra.numero == idOrdenCompra &&
		almacenTerminados[numeroEspacio].ordenProduccion.tipoProducto == idProducto)
	{
	    if(primeraCajaEncontrada == true)
	        return false;
	    else
	        primeraCajaEncontrada = true;
		
	    *caja = almacenTerminados[numeroEspacio].caja;
	    almacenTerminados[numeroEspacio]..estado = VACIO;
	}
    }
    return true;
}

void SmMemAlmacenProductosTerminados::asignarVaciosComoDisponibles(OrdenDeProduccion ordenProduccion, int cantidad)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO)
	{
	    almacenTerminados[numeroEspacio].ordenProduccion = ordenProduccion;
	    almacenTerminados[numeroEspacio].estado = RESERVADO_DISPONIBLE;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarVaciosAProduccion(OrdenDeProduccion ordenProduccion, OrdenDeCompra ordenCompra, int cantidad)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO)
	{
	    almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
	    almacenTerminados[numeroEspacio].ordenCompra = ordenCompra;
	    almacenTerminados[numeroEspacio].ordenProduccion = ordenProduccion;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarStockeados(OrdenDeCompra ordenCompra, int tipo, int cantidad)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == RESERVADO_DISPONIBLE &&
		almacenTerminados[numeroEspacio].ordenProduccion.tipoProducto == tipo)
	{
	    almacenTerminados[numeroEspacio].ordenCompra = ordenCompra;
	    almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
	    cantidad--;	    
	}
	
	if(cantidad <= 0)
	    return;
    }
}