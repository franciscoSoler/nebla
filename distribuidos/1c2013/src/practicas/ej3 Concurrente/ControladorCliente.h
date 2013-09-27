#ifndef CONTROLADOR_CLIENTE_H_
#define CONTROLADOR_CLIENTE_H_

#include "./IPC/SharedMemory/TicketsInfoSharedMemory.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "./IPC/MessageQueue/ComprarMessageQueue.h"
#include "./IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "./IPC/MessageQueue/VenderMessageQueue.h"

class ControladorCliente
{
private:
	
	GetVendedorMessageQueue	getVendedorQueue;
	ComprarMessageQueue comprarQueue;
	VenderMessageQueue venderQueue;

	
public:

	ControladorCliente(); 

	virtual ~ControladorCliente();

	int obtenerVendedor(int nroCliente);
	ResultadoCompra comprarTicket(int nroCliente, int nroVendedor, int dinero);
	void  finalizarCompra(int nroVendedor);

};

#endif // CONTROLADOR_CLIENTE_H_
