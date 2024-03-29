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

#include <Comunicaciones/Objects/CommSemaphoreMutex.h>

#include <IPCs/IPCTemplate/SharedMemory.h>
#include "../Objects/SmMemAlmacenProductosTerminados.h"
#include "../../Common.h"
#include "../../Parser/Parser.h"

#include <Comunicaciones/Objects/CommMsgHandler.h>

class ControladorVendedor {
public:
    ControladorVendedor();
    virtual ~ControladorVendedor();

    long obtenerNumeroVendedor();
    void vendedorLibre();
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
    IPC::SharedMemory<int> shmemNumeroOrdenCompra;
    IPC::SharedMemory<AlmacenProductosTerminados> shmemAlmacenTerminados;

    COMM::CommMsgHandler colaEnvioOrdenProduccion;

    COMM::CommSemaphoreMutex<AlmacenProductosTerminados> mutexAlmacenTerminados;
    COMM::CommSemaphoreMutex<int> mutexOrdenDeCompra;
    SmMemAlmacenProductosTerminados almacenProductosTerminados;
    COMM::CommMsgHandler inputQueueDespacho;

    void inicializarControlador();
    pedido_fabricacion_t reservarPedido(pedido_t pedido, bool* pedidoEnStock);
    void efectuarReserva(pedido_t pedido, pedido_fabricacion_t pedidoProduccion);
    int obtenerCantidadMinimaDeProduccion(int numProducto);
    void buscarUbicacionDeProductoEnArchivo(Parser & parser, ifstream& stream, int numeroProducto);

    void confirmarPedidos(pedido_fabricacion_t pedidos[], OrdenDeCompra ordenDeCompra, int cantPedidos);

    void enviarOrdenDeCompraDespacho(OrdenDeCompra ordenDeCompra);

    void enviarPedidoProduccionAAlmacenPiezas(pedido_fabricacion_t pedidoProduccion);
};

#endif	/* CONTROLADORVENDEDOR_H */

