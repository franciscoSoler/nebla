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

#include "../../IPCs/IPCAbstractos/MessageQueue/VendedoresMessageQueue.h"
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
	ControladorVendedor();
	ControladorVendedor(long numVendedor);
	virtual ~ControladorVendedor();
	
	int obtenerNumeroDeOrdenDeCompra();
	
	long recibirLlamadoTelefonico();
	pedido_t recibirPedido();
    void enviarRespuestaDePedido(long numCliente, respuesta_pedido_t respuesta);
	
	pedido_fabricacion_t reservarPedido(pedido_t pedido);
	void enviarPedidoProduccionAAlmacenPiezas(pedido_fabricacion_t pedidoProduccion);
	void terminarLlamadoTelefonico();
        
        void enviarOrdenDeCompraDespacho(OrdenDeCompra ordenDeCompra);
        
	void confirmarPedido(pedido_fabricacion_t pedidoProduccion, OrdenDeCompra ordenDeCompra);
	void anularPedidos();
	
    private:
	long numVendedor;
	IPC::VendedoresMessageQueue vendedores;
	IPC::ClientesMessageQueue clientes;
    IPC::PedidosVendedorMessageQueue pedidos;
	MemoriaCompartida shmemNumeroOrdenCompra;
	int* numeroOrdenCompra;
 
	Cola<mensaje_pedido_fabricacion_t> colaEnvioOrdenProduccion;

	IPC::Semaphore mutexAlmacenTerminados;
    IPC::Semaphore mutexOrdenDeCompra;
	SmMemAlmacenProductosTerminados almacenProductosTerminados;
    IPC::MsgQueue outputQueueDespacho;
	int obtenerCantidadMinimaDeProduccion(int numProducto);
	pedido_fabricacion_t calcularCantidadAProducir(pedido_t pedido);
	void efectuarReserva(pedido_t pedido, pedido_fabricacion_t pedidoProduccion);
        
        void buscarUbicacionDeProductoEnArchivo(Parser parser, ifstream& stream, int numeroProducto);
};

#endif	/* CONTROLADORVENDEDOR_H */

