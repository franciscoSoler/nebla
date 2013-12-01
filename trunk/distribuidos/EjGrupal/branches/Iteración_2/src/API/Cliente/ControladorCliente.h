/* 
 * File:   ControladorCliente.h
 * Author: fede
 *
 * Created on November 2, 2013, 12:58 AM
 */

#ifndef CONTROLADORCLIENTE_H
#define	CONTROLADORCLIENTE_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>

#include "../../Common.h"
#include "../../IPCs/Barrios/Cola.h"

#include "../../IPCs/IPCAbstractos/MessageQueue/VendedorLibreMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <Logger/Logger.h>

class ControladorCliente
{
public:
    ControladorCliente(long numCliente);
    virtual ~ControladorCliente();
	
    int obtenerNumeroCliente();
    
    void contactarVendedores();
    void enviarPedido(int cantidadUnidades, int tipo);
    void finalizarEnvio(int cantPedidos);
    respuesta_pedido_t recibirResultado();
    void esperarPedido(TipoProducto & tipoProducto, int & nroOrdenCompra, int numCliente);
    void retirarEncargo(TipoProducto tipo, int nroOrden, bool ultimoPedido);
    Caja obtenerProducto(int idCliente);

private:
        
    // IPC::VendedorLibreMessageQueue vendedores;
    IPC::ClientesMessageQueue clientes;
    IPC::PedidosVendedorMessageQueue pedidos;

    IPC::MsgQueue inputQueueCliente;
    IPC::MsgQueue inputQueueDespacho;
    IPC::MsgQueue R16_Cliente_Queue_;

    long numCliente;
    long numVendedorAsociado;
    int nroOrdenDeCompra;
    int cantidadProductos;        
    char mensajePantalla[255];
};

#endif	/* CONTROLADORCLIENTE_H */

