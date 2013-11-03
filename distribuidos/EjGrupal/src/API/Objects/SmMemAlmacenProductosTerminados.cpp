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
	    if(almacenTerminados[numeroEspacio].tipoProducto == numProducto)
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
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].ordenCompra.numero == idOrdenCompra &&
		almacenTerminados[numeroEspacio].tipoProducto == idProducto)
	{
	    almacenTerminados[numeroEspacio].caja = *caja;
	    return true;
	}
    }
    return false;
}

void SmMemAlmacenProductosTerminados::reservarVaciosComoDisponibles(int cantidad)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == false)
	{
	    almacenTerminados[numeroEspacio].esTemporal = true;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::reservarVaciosAProduccion(int cantidad)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == false)
	{
	    almacenTerminados[numeroEspacio].esTemporal = true;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::reservarStockeados(int tipo, int cantidad)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if((almacenTerminados[numeroEspacio].estado == RESERVADO_DISPONIBLE || almacenTerminados[numeroEspacio].estado == PRODUCTO_DISPONIBLE) &&
		almacenTerminados[numeroEspacio].tipoProducto == tipo &&
		almacenTerminados[numeroEspacio].esTemporal == false)
	{
	    almacenTerminados[numeroEspacio].esTemporal = true;
	    cantidad--;	    
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarVaciosComoDisponibles(int cantidad)
{
    OrdenDeCompra ordenDeCompra;
    ordenDeCompra.numero = 0;
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == true)
	{
	    almacenTerminados[numeroEspacio].ordenCompra = ordenDeCompra;
	    almacenTerminados[numeroEspacio].estado = RESERVADO_DISPONIBLE;
	    almacenTerminados[numeroEspacio].esTemporal = false;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarVaciosAProduccion(OrdenDeCompra ordenCompra, int cantidad)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == true)
	{
	    almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
	    almacenTerminados[numeroEspacio].ordenCompra = ordenCompra;
	    almacenTerminados[numeroEspacio].esTemporal = false;
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
		almacenTerminados[numeroEspacio].tipoProducto == tipo &&
		almacenTerminados[numeroEspacio].esTemporal == true)
	{
	    almacenTerminados[numeroEspacio].ordenCompra = ordenCompra;
	    almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
	    almacenTerminados[numeroEspacio].esTemporal = false;
	    cantidad--;	    
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::anularReservas()
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].esTemporal == true)
	{
	    almacenTerminados[numeroEspacio].esTemporal == false;
	} 
    } 
}