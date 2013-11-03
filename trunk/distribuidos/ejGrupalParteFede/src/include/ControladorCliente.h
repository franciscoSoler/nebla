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
#include "const.h"
#include "Cola.h"
#include <sys/types.h>

class ControladorCliente
{
    public:
	ControladorCliente();
	ControladorCliente(long numCliente);
	void contactarVendedores();
	void enviarPedido(int cantidadUnidades, int tipo, int numMensaje);
	void finalizarEnvio(int cantPedidos);
	virtual ~ControladorCliente();
    
    private:
	Cola<mensaje_inicial_t> vendedores;
	Cola<mensaje_inicial_t> clientes;
	Cola<pedido_t> pedidos;

	long numCliente;
	long numVendedorAsociado;
};

#endif	/* CONTROLADORCLIENTE_H */

