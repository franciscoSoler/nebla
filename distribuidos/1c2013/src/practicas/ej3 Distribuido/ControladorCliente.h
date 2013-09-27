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

	int nroVendedor;
	int nroCliente;
	
	char host[TAM_BUFFER];
	
public:

	ControladorCliente(int numero); 

	virtual ~ControladorCliente();

	//int obtenerNumeroCliente();
	void obtenerVendedor();
	ResultadoCompra comprarTicket(int dinero);
	void  finalizarCompra();

};

#endif // CONTROLADOR_CLIENTE_H_
