
#include "ControladorCliente.h"
#include "common.h"

ControladorCliente::ControladorCliente () {
	// Obteniendo las colas de mensajes
	getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR);
	
	comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR);

	venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER);
}

ControladorCliente::~ControladorCliente () {
}

int ControladorCliente::obtenerVendedor(int nroCliente) {
	HandShakeMessage handShake;
	handShake.mtype = 1;
	handShake.id = nroCliente;
	
	getVendedorQueue.enviarPedido(handShake);
	
	venderQueue.recibirNroVendedor (nroCliente,&handShake);
	int nroVendedor = handShake.id;

	return nroVendedor;
}

ResultadoCompra ControladorCliente::comprarTicket(int nroCliente, int nroVendedor, int dinero) {
	RequestTicketsMessage pedido;
	pedido.mtype = nroVendedor;
	pedido.ticketsInfo.cantidad = 1;
	pedido.ticketsInfo.dinero = dinero;
	comprarQueue.enviarPedido(pedido);
	
	ResponseTicketsMessage resultadoResponse;
	venderQueue.recibirResultado(nroCliente, &resultadoResponse);
	
	ResultadoCompra resultadoCompra = resultadoResponse.resultadoCompra;
	/*resultadoCompra.resultado = resultadoResponse.resultadoCompra.resultado;
	resultadoCompra.vuelto = resultadoResponse.resultadoCompra.vuelto;
	strcpy(resultadoCompra.mensaje, resultadoResponse.resultadoCompra.mensaje);*/
	
	return resultadoCompra;
}

void  ControladorCliente::finalizarCompra(int nroVendedor) {
	// Le envio un mensaje al vendedor para indicarle que no compro mas tickets
	RequestTicketsMessage pedido;
	pedido.mtype = nroVendedor;
	pedido.ticketsInfo.cantidad = 0;
	pedido.ticketsInfo.dinero = 0;
	comprarQueue.enviarPedido(pedido);
}
