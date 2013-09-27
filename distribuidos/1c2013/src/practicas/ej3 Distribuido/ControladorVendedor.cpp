
#include "ControladorVendedor.h"
#include "common.h"

ControladorVendedor::ControladorVendedor (int numVendedor) {
	
	nroVendedor = numVendedor;
	
	// Obteniendo las colas de mensajes
	getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR_1);
	
	comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR_1);

	venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER_1);
}

ControladorVendedor::~ControladorVendedor () {
}

void ControladorVendedor::enviarVendedorLibre() {
	HandShakeMessage handShake;
		
	handShake.mtype = 1;
	handShake.numero = nroVendedor;
	getVendedorQueue.enviarPedido(handShake);
	
	//comprarQueue.recibirNroCliente(nroVendedor, &handShake);
	//nroCliente = handShake.numero;
}

TicketsInfo ControladorVendedor::obtenerPedido() {
	TicketsInfo returnValue;
	RequestTicketsMessage request;
	int resultado = comprarQueue.recibirPedido(nroVendedor,&request);
	
	if (resultado < 0) returnValue.cantidad = -1;
	else returnValue = request.ticketsInfo;

	nroCliente = request.origen;

	return returnValue;			
}

int ControladorVendedor::enviarResultado(ResultadoCompra resultado) {
	ResponseTicketsMessage response;
	response.mtype = nroCliente;
	response.origen = nroVendedor;
	response.tipo = 0;
	response.resultadoCompra = resultado;
		
	return venderQueue.enviarResultado(response);		
}

void ControladorVendedor::finalizarVenta() {
	ResponseTicketsMessage response;
	response.mtype = nroCliente;
	response.origen = nroVendedor;
	response.tipo = 1;
		
	venderQueue.enviarResultado(response);		
}
