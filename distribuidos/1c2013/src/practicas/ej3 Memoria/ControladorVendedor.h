#ifndef CONTROLADOR_VENDEDOR_H_
#define CONTROLADOR_VENDEDOR_H_

#include "./IPC/MessageQueue/ComprarMessageQueue.h"
#include "./IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "./IPC/MessageQueue/VenderMessageQueue.h"

class ControladorVendedor {
private:
	GetVendedorMessageQueue	getVendedorQueue;
	ComprarMessageQueue comprarQueue;
	VenderMessageQueue venderQueue;
	
	int nroVendedor;
	int nroCliente;
	
public:
	ControladorVendedor(int numVendedor); 
	virtual ~ControladorVendedor();

	void enviarVendedorLibre();
	TicketsInfo obtenerPedido();
	int enviarResultado(ResultadoCompra resultado);
	void finalizarVenta();
};

#endif // CONTROLADOR_VENDEDOR_H_
