/* 
 * File:   SmMemAlmacenProductosTerminados.h
 * Author: fede
 *
 * Created on November 2, 2013, 10:34 PM
 */

#ifndef SMMEMALMACENPRODUCTOSTERMINADOS_H
#define	SMMEMALMACENPRODUCTOSTERMINADOS_H

//#include "MemoriaCompartida.h"
#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../Common.h"

class SmMemAlmacenProductosTerminados
{
public:
	SmMemAlmacenProductosTerminados();
	virtual ~SmMemAlmacenProductosTerminados();
	
        void establecerMatriz(EspacioAlmacenProductos almacenTerminados[TAM_ALMACEN]);
        
	int obtenerEspaciosVacios();
	int obtenerCantidadDeStockDeProducto(int numProducto);
	
	void asignarVaciosComoDisponibles(int cantidad, int tipoProducto);
	
	void reservarVaciosComoDisponibles(int cantidad);
	void reservarVaciosAProduccion(int cantidad);
	void reservarStockeados(int tipo, int cantidad);
	
	void anularReservas();

    // FIXME: APT nunca necesita la orden de compra, solo el ID. La ODC se almacena en Despacho
    void asignarVaciosAProduccion(long idOrdenCompra, int cantidad, int tipoProducto);
    // FIXME: APT nunca necesita la orden de compra, solo el ID. La ODC se almacena en Despacho
    void asignarStockeados(long idOrdenCompra, int cantidad, int tipoProducto);
	
    /* Retorna true si la Caja depositada fue la última del pedido de la misma */
    bool depositarCaja(Caja caja, long & idOrdenDeCompra);
    bool sacarCaja(Caja* caja, int idProducto, int idOrdenCompra);
    bool chequearPedidoReservadoEnStock(int tipoProducto);
	
	void obtenerMatriz(EspacioAlmacenProductos almacenTerminados[TAM_ALMACEN]);

private:
	//MemoriaCompartida shmemAlmacenTerminados;
	EspacioAlmacenProductos almacenTerminados[TAM_ALMACEN];	

    void almacenarProductoNoReservadoEnAlmacen(Caja unaCaja, long & idOrdenDeCompra);
    void almacenarProductoReservadoEnAlmacen(Caja unaCaja);
    bool pedidoFueCompletado(TipoProducto tipo, const long idOrdenDeCompra) const;
};

#endif	/* SMMEMALMACENPRODUCTOSTERMINADOS_H */

