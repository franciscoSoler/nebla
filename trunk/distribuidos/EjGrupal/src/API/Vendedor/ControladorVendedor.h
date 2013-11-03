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

#include "../../IPCs/Semaphore/Semaphore.h"

#include "../../IPCs/Barrios/Cola.h"
#include "../../IPCs/Barrios/MemoriaCompartida.h"
#include "../Objects/SmMemAlmacenProductosTerminados.h"
#include "../../Common.h"
#include <IPCs/IPCTemplate/MsgQueue.h>

class ControladorVendedor
{
    public:
	ControladorVendedor();
	ControladorVendedor(long numVendedor);
	virtual ~ControladorVendedor();
	
	int obtenerNumeroDeOrdenDeCompra();
	
	mensaje_inicial_t recibirLlamadoTelefonico();
	pedido_t recibirPedido();
	void enviarRespuestaDePedido(long numCliente, bool resultado);
	
	pedido_produccion_t reservarPedido(pedido_t pedido);
	void enviarPedidoProduccionAAlmacenPiezas(pedido_produccion_t pedidoProduccion);
	void terminarLlamadoTelefonico();
        
        void enviarOrdenDeCompraDespacho(OrdenDeCompra ordenDeCompra);
	
	void confirmarPedido(pedido_produccion_t pedidoProduccion, OrdenDeCompra ordenDeCompra);
	void anularPedidos();
	
    private:
	long numVendedor;
	Cola<mensaje_inicial_t> vendedores;
	Cola<respuesta_pedido_t> clientes;
	Cola<pedido_t> pedidos;
	MemoriaCompartida shmemNumeroOrdenCompra;
	int* numeroOrdenCompra;
	Cola<consulta_almacen_piezas_t> consultasAlmacen;
	Cola<respuesta_almacen_piezas_t> respuestasAlmacen;
	IPC::Semaphore mutexAlmacenTerminados;
	SmMemAlmacenProductosTerminados almacenProductosTerminados;
        
        IPC::MsgQueue outputQueueDespacho;
	
	int obtenerCantidadMinimaDeProduccion(int numProducto);
	pedido_produccion_t calcularCantidadAProducir(pedido_t pedido);
	void efectuarReserva(pedido_t pedido, pedido_produccion_t pedidoProduccion);
};

#endif	/* CONTROLADORVENDEDOR_H */

