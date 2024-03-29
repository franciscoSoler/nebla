/* 
 * File:   SmMemAlmacenProductosTerminados.h
 * Author: fede
 *
 * Created on November 2, 2013, 10:34 PM
 */

#ifndef SMMEMALMACENPRODUCTOSTERMINADOS_H
#define	SMMEMALMACENPRODUCTOSTERMINADOS_H

#include "MemoriaCompartida.h"
#include "Semaforo.h"
#include "const.h"

class SmMemAlmacenProductosTerminados
{
    public:
	SmMemAlmacenProductosTerminados();
	virtual ~SmMemAlmacenProductosTerminados();
	
	int obtenerEspaciosVacios();
	int obtenerCantidadDeStockDeProducto(int numProducto);
	
	void asignarVaciosComoDisponibles(OrdenDeProduccion ordenProduccion, int cantidad);
	void asignarVaciosAProduccion(OrdenDeProduccion ordenProduccion, OrdenDeCompra ordenCompra, int cantidad);
	void asignarStockeados(OrdenDeCompra ordenCompra, int tipo, int cantidad);
	
	bool depositarCaja(Caja caja);
	bool sacarCaja(Caja* caja, int idProducto, int idOrdenCompra);
    private:
	MemoriaCompartida shmemAlmacenTerminados;
	EspacioAlmacenProductos* almacenTerminados;	
};

#endif	/* SMMEMALMACENPRODUCTOSTERMINADOS_H */

