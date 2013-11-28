/* 
 * File:   SmMemAlmacenProductosTerminados.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 10:34 PM
 */

#include "SmMemAlmacenProductosTerminados.h"
#include <Logger/Logger.h>

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
    // En la inserción, se de prioridad a aquellos productos RESERVADO_VENDIDO
    bool cajaAlmacenada = false;
    int i = 0;
    while ( !cajaAlmacenada && i < TAM_ALMACEN ) {
        if (almacenTerminados[i].estado == RESERVADO_VENDIDO
            && almacenTerminados[i].tipoProducto == caja.idProducto_) {
            
            char buffer_[TAM_BUFFER];
            sprintf(buffer_, "Deposito Caja en APT - TipoProducto: %d en el lugar %d, idNroOrdenVieja: %ld idNroOrdenNueva: %ld", caja.idProducto_, i, caja.idOrdenDeCompra_, almacenTerminados[i].idOrdenDeCompra);
            Logger::logMessage(Logger::TRACE, buffer_);
            almacenTerminados[i].caja = caja;
            almacenTerminados[i].estado = PRODUCTO_PARA_ENTREGAR;
            idOrdenDeCompra = almacenTerminados[i].idOrdenDeCompra;
            cajaAlmacenada = true;
        }
        i++;
    }
    
    i = 0;
    while ( !cajaAlmacenada && i < TAM_ALMACEN ) {
        if (almacenTerminados[i].estado == RESERVADO_DISPONIBLE
            && almacenTerminados[i].tipoProducto == caja.idProducto_) {

            almacenTerminados[i].caja = caja;
            almacenTerminados[i].estado = PRODUCTO_DISPONIBLE;
            char mensajePantalla[TAM_BUFFER];
            sprintf(mensajePantalla, "La caja se guardo en el APT como disponible - "
                    "idProducto: %d, idOrdenDeCompraDeLaCaja: %ld, "
                    "idOrdenDeCompra: %ld", caja.idProducto_, 
                    caja.idOrdenDeCompra_, idOrdenDeCompra);
            Logger::logMessage(Logger::TRACE, mensajePantalla);
            return false;
        }
        i++;
    }
    
    if ( not cajaAlmacenada) {
        char mensajePantalla[TAM_BUFFER];
        sprintf(mensajePantalla, "La caja no fue guardada en el APT - "
                "idProducto: %d, idOrdenDeCompraDeLaCaja: %ld, "
                "idOrdenDeCompra: %ld", caja.idProducto_, 
                caja.idOrdenDeCompra_, idOrdenDeCompra);
        Logger::logMessage(Logger::ERROR, mensajePantalla);
    }
    
    return pedidoFueCompletado(caja.idProducto_, idOrdenDeCompra);
 /*   
    // TODO: Los ids SIEMPRE tienen que ser unsigned. Crear un tipo nuevo en definedTypes
    // y utilizarlo para todos los IDs
    idOrdenDeCompra = caja.idOrdenDeCompra_;

    if ( caja.idOrdenDeCompra_ == 0) {
        almacenarProductoNoReservadoEnAlmacen(caja, idOrdenDeCompra);

        if (! idOrdenDeCompra ) {
            // El producto no cambió su condición durante el transcurso del ensamblaje
            return false;
        }
    }
    else {
        almacenarProductoReservadoEnAlmacen(caja);
    }

    return pedidoFueCompletado(caja.idProducto_, idOrdenDeCompra);*/
}

void SmMemAlmacenProductosTerminados::almacenarProductoNoReservadoEnAlmacen(Caja unaCaja, long & idOrdenDeCompra) {
    // En la inserción, se de prioridad a aquellos productos RESERVADO_VENDIDO
    for (int i = 0; i < TAM_ALMACEN; ++i) {
        if (almacenTerminados[i].estado == RESERVADO_VENDIDO
            && almacenTerminados[i].tipoProducto == unaCaja.idProducto_) {

            almacenTerminados[i].caja = unaCaja;
            almacenTerminados[i].estado = PRODUCTO_PARA_ENTREGAR;
            idOrdenDeCompra = almacenTerminados[i].idOrdenDeCompra;
            return;
        }
    }
    
    
    for (int i = 0; i < TAM_ALMACEN; ++i) {
        if (almacenTerminados[i].estado == RESERVADO_DISPONIBLE
            && almacenTerminados[i].tipoProducto == unaCaja.idProducto_) {

            almacenTerminados[i].caja = unaCaja;
            almacenTerminados[i].estado = PRODUCTO_DISPONIBLE;
            return;
        }
    }

    
}

void SmMemAlmacenProductosTerminados::almacenarProductoReservadoEnAlmacen(Caja unaCaja) {
    for (int i = 0; i < TAM_ALMACEN; ++i) {
        if (almacenTerminados[i].estado == RESERVADO_VENDIDO
            && almacenTerminados[i].tipoProducto == unaCaja.idProducto_
            && almacenTerminados[i].idOrdenDeCompra == unaCaja.idOrdenDeCompra_) {

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
            && almacenTerminados[i].idOrdenDeCompra == idOrdenDeCompra) {

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
        if(almacenTerminados[numeroEspacio].idOrdenDeCompra == idOrdenCompra &&
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

bool SmMemAlmacenProductosTerminados::chequearPedidoReservadoEnStock(int tipoProducto) {
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == RESERVADO_DISPONIBLE &&
            almacenTerminados[numeroEspacio].tipoProducto == tipoProducto &&
            almacenTerminados[numeroEspacio].esTemporal == true)
        {
            char mensajePantalla[TAM_BUFFER];
            sprintf(mensajePantalla, "chequearPedidoReservadoEnStock - el "
                    "producto %d no esta completamente en stock, la posicion: "
                    "%d esta en produccion", tipoProducto, numeroEspacio);
            Logger::logMessage(Logger::TRACE, mensajePantalla);
            return false;
        }
    }
    return true;
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
        if((almacenTerminados[numeroEspacio].estado == RESERVADO_DISPONIBLE
	    || almacenTerminados[numeroEspacio].estado == PRODUCTO_DISPONIBLE)
	    && almacenTerminados[numeroEspacio].tipoProducto == tipo
	    && almacenTerminados[numeroEspacio].esTemporal == false)
	{
	    almacenTerminados[numeroEspacio].esTemporal = true;
	    almacenTerminados[numeroEspacio].tipoProducto = tipo;
	    cantidad--;	   
            char mensajePantalla[1024];
            sprintf(mensajePantalla, "Se RESERVAN los productos stockeados: %d de la posicion %d", tipo, numeroEspacio);
            Logger::logMessage(Logger::TRACE, mensajePantalla);
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarVaciosComoDisponibles(int cantidad, int tipoProducto)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == true)
	{
            almacenTerminados[numeroEspacio].idOrdenDeCompra = 0;
	    almacenTerminados[numeroEspacio].estado = RESERVADO_DISPONIBLE;
	    almacenTerminados[numeroEspacio].esTemporal = false;
	    almacenTerminados[numeroEspacio].tipoProducto = tipoProducto;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarVaciosAProduccion(long idOrdenCompra, int cantidad, int tipoProducto)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
	if(almacenTerminados[numeroEspacio].estado == VACIO &&
		almacenTerminados[numeroEspacio].esTemporal == true)
	{
	    almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
	    almacenTerminados[numeroEspacio].idOrdenDeCompra = idOrdenCompra;
	    almacenTerminados[numeroEspacio].esTemporal = false;
	    almacenTerminados[numeroEspacio].tipoProducto = tipoProducto;
	    cantidad--;
	}
	
	if(cantidad <= 0)
	    return;
    }
}

void SmMemAlmacenProductosTerminados::asignarStockeados(long idOrdenCompra, int cantidad, int tipoProducto)
{
    for(int numeroEspacio = 0; numeroEspacio < TAM_ALMACEN; numeroEspacio++)
    {
        if(almacenTerminados[numeroEspacio].estado == RESERVADO_DISPONIBLE &&
            almacenTerminados[numeroEspacio].tipoProducto == tipoProducto &&
            almacenTerminados[numeroEspacio].esTemporal == true)
        {
            almacenTerminados[numeroEspacio].idOrdenDeCompra = idOrdenCompra;
            almacenTerminados[numeroEspacio].estado = RESERVADO_VENDIDO;
            almacenTerminados[numeroEspacio].esTemporal = false;
            almacenTerminados[numeroEspacio].tipoProducto = tipoProducto;
            cantidad--;
            char mensajePantalla[1024];
            sprintf(mensajePantalla, "Se ASIGNAN como RESERVADO_VENDIDO los productos RESERVADO_DISPONIBLE: %d de la posicion %d", tipoProducto, numeroEspacio);
            Logger::logMessage(Logger::TRACE, mensajePantalla);
        }
	
	else if(almacenTerminados[numeroEspacio].estado == PRODUCTO_DISPONIBLE &&
            almacenTerminados[numeroEspacio].tipoProducto == tipoProducto &&
            almacenTerminados[numeroEspacio].esTemporal == true)
        {
            almacenTerminados[numeroEspacio].idOrdenDeCompra = idOrdenCompra;
            almacenTerminados[numeroEspacio].estado = PRODUCTO_PARA_ENTREGAR;
            almacenTerminados[numeroEspacio].esTemporal = false;
            almacenTerminados[numeroEspacio].tipoProducto = tipoProducto;
            cantidad--;
            char mensajePantalla[1024];
            sprintf(mensajePantalla, "Se ASIGNAN como PRODUCTO_PARA_ENTREGAR los productos PRODUCTO_DISPONIBLE: %d de la posicion %d", tipoProducto, numeroEspacio);
            Logger::logMessage(Logger::TRACE, mensajePantalla);
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
