
#include "ControladorVendedor.h"
#include "common.h"

ControladorVendedor::ControladorVendedor (int numVendedor) {
	
	nroVendedor = numVendedor;
	
	// Obteniendo las colas de mensajes
	getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR);
	
	comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR);

	venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER);
}

ControladorVendedor::~ControladorVendedor () {
}

int ControladorVendedor::obtenerCliente() {
	HandShakeMessage handShake;
	int resultado = getVendedorQueue.recibirPedido(1,&handShake);
	if (resultado < 0) return resultado;
	
	int nroCliente = handShake.id;
	
	handShake.mtype = nroCliente;
	handShake.id = nroVendedor;
	resultado = venderQueue.enviarNroVendedor(handShake);
	if (resultado < 0) return resultado;
	
	return nroCliente;
}

TicketsInfo ControladorVendedor::obtenerPedido() {
	TicketsInfo returnValue;
	RequestTicketsMessage request;
	int resultado = comprarQueue.recibirPedido(nroVendedor,&request);
	
	if (resultado < 0) returnValue.cantidad = -1;
	else returnValue = request.ticketsInfo;
		
	return returnValue;			
}

int ControladorVendedor::enviarResultado(int nroCliente, ResultadoCompra resultado) {
	ResponseTicketsMessage response;
	response.mtype = nroCliente;
	response.resultadoCompra = resultado;
		
	return venderQueue.enviarResultado(response);
			
}
