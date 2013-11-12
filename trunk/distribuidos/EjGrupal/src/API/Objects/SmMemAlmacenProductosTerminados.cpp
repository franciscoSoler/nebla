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

EspacioAlmacenProductos* SmMemAlmacenProductosTerminados::obtenerMatriz()
{
    return this->almacenTerminados;
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

bool SmMemAlmacenProductosTerminados::depositarCaja(Caja caja, long & idOrdenDeCompra)
{
    // TODO: Los ids SIEMPRE tienen que ser unsigned. Crear un tipo nuevo en definedTypes
    // y utilizarlo para todos los IDs
    idOrdenDeCompra = caja.idOrdenDeCompra_;

    if ( caja.idOrdenDeCompra_ == 0) {
        almacenarProductoNoReservadoEnAlmacen(caja, idOrdenDeCompra);
        return false;
    }
    else {
        almacenarProductoReservadoEnAlmacen(caja);
    }

    return pedidoFueCompletado(caja.idProducto_, idOrdenDeCompra);
}

void SmMemAlmacenProductosTerminados::almacenarProductoNoReservadoEnAlmacen(Caja unaCaja, long & idOrdenDeCompra) {
    // En la inserción, se de prioridad a aquellos productos reservados disponibles
    for (int i = 0; i < TAM_ALMACEN; ++i) {
        if (almacenTerminados[i].estado == RESERVADO_DISPONIBLE
            && almacenTerminados[i].tipoProducto == unaCaja.idProducto_) {

            almacenTerminados[i].caja = unaCaja;
            almacenTerminados[i].estado = PRODUCTO_DISPONIBLE;
            return;
        }
    }

    // El producto fue vendido durante la producción, se procede a almacenar el mismo en algún
    // lugar del almacen que sea de su mismo tipo y que posea el estado RESERVADO_VENDIDO
    for (int i = 0; i < TAM_ALMACEN; ++i) {
        if (almacenTerminados[i].estado == RESERVADO_VENDIDO
            && almacenTerminados[i].tipoProducto == unaCaja.idProducto_) {

            almacenTerminados[i].caja = unaCaja;
            almacenTerminados[i].estado = PRODUCTO_PARA_ENTREGAR;
            idOrdenDeCompra = almacenTerminados[i].ordenCompra.idOrden_;
            return;
        }
    }
}

void SmMemAlmacenProductosTerminados::almacenarProductoReservadoEnAlmacen(Caja unaCaja) {
    for (int i = 0; i < TAM_ALMACEN; ++i) {
        if (almacenTerminados[i].estado == RESERVADO_VENDIDO
            && almacenTerminados[i].tipoProducto == unaCaja.idProducto_
            && almacenTerminados[i].ordenCompra.idOrden_ == unaCaja.idOrdenDeCompra_) {

            almacenTerminados[i].caja = unaCaja;
            almacenTerminados[i].estado = PRODUCTO_PARA_ENTREGAR;
            break;
        }
    }
}

bool SmMemAlmacenProductosTerminados::pedidoFueCompletado(TipoProducto tipo, const long idOrdenDeCompra) const {
    bool pedidoCompletado = true;

    for (int i = 0; i < TAM_ALMACEN; ++i) {
        if (almacenTerminados[i].tipoProducto == tipo
            && almacenTerminados[i].ordenCompra.idOrden_ == idOrdenDeCompra) {

            if (almacenTerminados[i].estado == RESERVADO_VENDIDO) {
                pedidoCompletado = false;
                continue;
            }
        }
    }

    return pedidoCompletado;
}

bool SmMemAlmacenProductosTerminados::sacarCaja(Caja* caja, int idProducto, int idOrdenCompra)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
        if(almacenTerminados[numeroEspacio].ordenCompra.idOrden_ == idOrdenCompra &&
            almacenTerminados[numeroEspacio].tipoProducto == idProducto &&
                almacenTerminados[numeroEspacio].estado == PRODUCTO_PARA_ENTREGAR)
        {
            *caja = almacenTerminados[numeroEspacio].caja;
            almacenTerminados[numeroEspacio].estado = VACIO;
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
	    almacenTerminados[numeroEspacio].tipoProducto = tipo;
	    cantidad--;	    
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarVaciosComoDisponibles(int cantidad, int tipoProducto)
{
    OrdenDeCompra ordenDeCompra;
    ordenDeCompra.idOrden_ = 0;
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == true)
	{
	    almacenTerminados[numeroEspacio].ordenCompra = ordenDeCompra;
	    almacenTerminados[numeroEspacio].estado = RESERVADO_DISPONIBLE;
	    almacenTerminados[numeroEspacio].esTemporal = false;
	    almacenTerminados[numeroEspacio].tipoProducto = tipoProducto;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarVaciosAProduccion(OrdenDeCompra ordenCompra, int cantidad, int tipoProducto)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == true)
	{
	    almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
        almacenTerminados[numeroEspacio].ordenCompra = ordenCompra;
	    almacenTerminados[numeroEspacio].esTemporal = false;
	    almacenTerminados[numeroEspacio].tipoProducto = tipoProducto;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarStockeados(OrdenDeCompra ordenCompra, int cantidad, int tipoProducto)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
        if(almacenTerminados[numeroEspacio].estado == RESERVADO_DISPONIBLE &&
            almacenTerminados[numeroEspacio].tipoProducto == tipoProducto &&
            almacenTerminados[numeroEspacio].esTemporal == true)
        {
            almacenTerminados[numeroEspacio].ordenCompra = ordenCompra;
            almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
            almacenTerminados[numeroEspacio].esTemporal = false;
            almacenTerminados[numeroEspacio].tipoProducto = tipoProducto;
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
            almacenTerminados[numeroEspacio].esTemporal = false;
        }
    }
}
