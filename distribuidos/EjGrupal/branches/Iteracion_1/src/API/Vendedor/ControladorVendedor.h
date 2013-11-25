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

#include "../../IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"

#include "../../IPCs/Semaphore/Semaphore.h"

#include "../../IPCs/Barrios/Cola.h"
#include "../../IPCs/Barrios/MemoriaCompartida.h"
#include "../Objects/SmMemAlmacenProductosTerminados.h"
#include "../../Common.h"
#include <IPCs/IPCTemplate/MsgQueue.h>
#include "../../Parser/Parser.h"

class ControladorVendedor
{
public:
	ControladorVendedor(long numVendedor);
	virtual ~ControladorVendedor();
	
    void recibirLlamadoTelefonico();
	int obtenerNumeroDeOrdenDeCompra();
	pedido_t recibirPedido();
	void enviarConfirmacionDeRecepcionDePedido(long numCliente, respuesta_pedido_t pedido);
    void enviarUltimaConfirmacionDeRecepcionDePedido(long numCliente, respuesta_pedido_t pedido);
	bool realizarOrdenDeCompra(pedido_t pedidos[], OrdenDeCompra* ordenDeCompra, int cantPedidos);
	void cancelarOrdenDeCompraACliente(long numCliente, respuesta_pedido_t respuesta);
    pedido_fabricacion_t calcularCantidadAProducir(pedido_t pedido, bool* pedidoEnStock);

private:
	long numVendedor;
	IPC::VendedorLibreMessageQueue vendedores;
	IPC::ClientesMessageQueue clientes;
	IPC::PedidosVendedorMessageQueue pedidos;
	MemoriaCompartida shmemNumeroOrdenCompra;
	int* numeroOrdenCompra;
 
	Cola<mensaje_pedido_fabricacion_t> colaEnvioOrdenProduccion;

	IPC::Semaphore mutexAlmacenTerminados;
	IPC::Semaphore mutexOrdenDeCompra;
	SmMemAlmacenProductosTerminados almacenProductosTerminados;
    IPC::MsgQueue inputQueueDespacho;

    pedido_fabricacion_t reservarPedido(pedido_t pedido, bool* pedidoEnStock);
	void efectuarReserva(pedido_t pedido, pedido_fabricacion_t pedidoProduccion);
	int obtenerCantidadMinimaDeProduccion(int numProducto);
	void buscarUbicacionDeProductoEnArchivo(Parser parser, ifstream& stream, int numeroProducto);
	
	void confirmarPedidos(pedido_fabricacion_t pedidos[], OrdenDeCompra ordenDeCompra, int cantPedidos);
	
	void enviarOrdenDeCompraDespacho(OrdenDeCompra ordenDeCompra);

    void enviarPedidoProduccionAAlmacenPiezas(pedido_fabricacion_t pedidoProduccion);

};

#endif	/* CONTROLADORVENDEDOR_H */

