/* 
 * File:   SmMemAlmacenProductosTerminados.h
 * Author: fede
 *
 * Created on November 2, 2013, 10:34 PM
 */

#ifndef SMMEMALMACENPRODUCTOSTERMINADOS_H
#define	SMMEMALMACENPRODUCTOSTERMINADOS_H

#include "MemoriaCompartida.h"
#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../Common.h"

class SmMemAlmacenProductosTerminados
{
    public:
	SmMemAlmacenProductosTerminados();
	virtual ~SmMemAlmacenProductosTerminados();
	
	int obtenerEspaciosVacios();
	int obtenerCantidadDeStockDeProducto(int numProducto);
	
	void asignarVaciosComoDisponibles(int cantidad, int tipoProducto);
	void asignarVaciosAProduccion(OrdenDeCompra ordenCompra, int cantidad, int tipoProducto);
	void asignarStockeados(OrdenDeCompra ordenCompra, int cantidad, int tipoProducto);
	
	void reservarVaciosComoDisponibles(int cantidad);
	void reservarVaciosAProduccion(int cantidad);
	void reservarStockeados(int tipo, int cantidad);
	
	void anularReservas();
	
	bool depositarCaja(Caja caja);
	bool sacarCaja(Caja* caja, int idProducto, int idOrdenCompra);
	
	EspacioAlmacenProductos* obtenerMatriz();
    private:
	MemoriaCompartida shmemAlmacenTerminados;
	EspacioAlmacenProductos* almacenTerminados;	
};

#endif	/* SMMEMALMACENPRODUCTOSTERMINADOS_H */

