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

class ControladorCliente
{
    public:
	ControladorCliente();
	ControladorCliente(long numCliente);
	virtual ~ControladorCliente();
	
	void contactarVendedores();
	void enviarPedido(int cantidadUnidades, int tipo, int numMensaje);
	void finalizarEnvio(int cantPedidos);
	
	bool recibirResultado();
    
    private:
	Cola<mensaje_inicial_t> vendedores;
	Cola<respuesta_pedido_t> clientes;
	Cola<pedido_t> pedidos;

	long numCliente;
	long numVendedorAsociado;
};

#endif	/* CONTROLADORCLIENTE_H */

