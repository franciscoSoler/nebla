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

#include "../../IPCs/IPCAbstractos/MessageQueue/VendedoresMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <Logger/Logger.h>

class ControladorCliente
{
    public:
	ControladorCliente();
	ControladorCliente(long numCliente);
	virtual ~ControladorCliente();
	
    void contactarVendedores();

	void enviarPedido(int cantidadUnidades, int tipo);
	void finalizarEnvio(int cantPedidos);
    respuesta_pedido_t recibirResultado();
    void esperarPedido(TipoProducto & tipoProducto, int & nroOrdenCompra, int numCliente);
    void retirarEncargo(TipoProducto tipo, int nroOrden);

    Caja obtenerProducto(int idCliente);
        
    private:
        
    IPC::VendedoresMessageQueue vendedores;
	IPC::ClientesMessageQueue clientes;
    IPC::PedidosVendedorMessageQueue pedidos;

    IPC::MsgQueue inputQueueCliente;
    IPC::MsgQueue despacho;
    IPC::MsgQueue retiro;

	long numCliente;
	long numVendedorAsociado;
        
    int nroOrdenDeCompra;
    int cantidadProductos;        
    
    char mensajePantalla[255];
};

#endif	/* CONTROLADORCLIENTE_H */

