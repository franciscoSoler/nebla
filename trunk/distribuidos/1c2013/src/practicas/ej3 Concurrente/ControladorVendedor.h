#ifndef CONTROLADOR_VENDEDOR_H_
#define CONTROLADOR_VENDEDOR_H_

#include "./IPC/MessageQueue/ComprarMessageQueue.h"
#include "./IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "./IPC/MessageQueue/VenderMessageQueue.h"

class ControladorVendedor
{
private:
	GetVendedorMessageQueue	getVendedorQueue;
	ComprarMessageQueue comprarQueue;
	VenderMessageQueue venderQueue;
	
	int nroVendedor;
	
public:

	ControladorVendedor(int numVendedor); 

	virtual ~ControladorVendedor();

	int obtenerCliente();
	TicketsInfo obtenerPedido();
	int enviarResultado(int nroCliente, ResultadoCompra resultado);
};

#endif // CONTROLADOR_VENDEDOR_H_
