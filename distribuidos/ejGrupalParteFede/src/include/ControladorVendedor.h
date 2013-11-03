/* 
 * File:   ControladorVendedor.h
 * Author: fede
 *
 * Created on November 2, 2013, 12:57 AM
 */

#ifndef CONTROLADORVENDEDOR_H
#define	CONTROLADORVENDEDOR_H

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Cola.h"
#include "Semaforo.h"
#include "MemoriaCompartida.h"
#include "SmMemAlmacenProductosTerminados.h"
#include "const.h"

class ControladorVendedor
{
    public:
	ControladorVendedor();
	ControladorVendedor(long numVendedor);
	virtual ~ControladorVendedor();
	
	mensaje_inicial_t recibirLlamadoTelefonico();
	pedido_t recibirPedido();
	bool realizarPedido(pedido_t pedido);
	void informarExitoEnPedido(pedido_t pedido);
	void informarErrorEnPedido(pedido_t pedido);
	
    private:
	long numVendedor;
	Cola<mensaje_inicial_t> vendedores;
	Cola<mensaje_inicial_t> clientes;
	Cola<pedido_t> pedidos;
	MemoriaCompartida shmemNumeroOrdenProduccion;
	MemoriaCompartida shmemNumeroOrdenCompra;
	int* numeroOrdenCompra;
	int* numeroOrdenProduccion;
	Cola<consulta_almacen_piezas_t> consultasAlmacen;
	Cola<respuesta_almacen_piezas_t> respuestasAlmacen;
	Semaforo mutexAlmacenTerminados;
	SmMemAlmacenProductosTerminados almacenProductosTerminados;

	int obtenerCantidadMinimaDeProduccion(int numProducto);
	pedido_produccion_t calcularCantidadAProducir(pedido_t pedido);
	void reservarPedido(pedido_t pedido, pedido_produccion_t pedidoProduccion);
};

#endif	/* CONTROLADORVENDEDOR_H */

