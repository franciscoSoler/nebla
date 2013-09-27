
#include "ControladorCliente.h"

#include <iostream>
#include <fstream>

#include "common.h"

/*#include "Numerador/numerador.h"
#include "Numerador/utilsClientes.h"

extern int numerarCliente(char *host);
extern void devolverCliente(char *host, int numero);*/

ControladorCliente::ControladorCliente (int numero) {
	
	nroCliente = numero;
	
	// Obteniendo las colas de mensajes
	getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR_2);
	
	comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR_2);

	venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER_2);
	
	/*char server[TAM_BUFFER];
	int puertoEntrada = 0;
	int puertoSalida = 0;
	std::ifstream input ("Config", std::ifstream::in);
	if (! input) { 
		std::cerr << "Controlador Cliente: Error al abrir el archivo de configuracion.\n"; 
	}
	else {
		input >> server; // Unused
		input >> puertoEntrada; // Unused
		input >> puertoSalida; // Unused
		input >> host;
		input.close();
		
		std::cout << "Config: Server: "<<server<<" Entrada: "<<puertoEntrada<<" Salida: "<<puertoSalida<< " Numerador: "<<host<<std::endl; 
	}*/
}

ControladorCliente::~ControladorCliente () {
}

/*int ControladorCliente::obtenerNumeroCliente() {

	nroCliente = numerarCliente(host);
	
	char buffer[TAM_BUFFER];	
	sprintf(buffer, "Cliente: Obtuvo numero de cliente: %d\n", nroCliente);
	write(fileno(stdout), buffer, strlen(buffer));
	
	return nroCliente;
}*/

void ControladorCliente::obtenerVendedor() {
	HandShakeMessage handShake;
	handShake.mtype = 1;
	handShake.numero = nroCliente;
	
	getVendedorQueue.enviarPedido(handShake);
	
	venderQueue.recibirNroVendedor(nroCliente, &handShake);
	nroVendedor = handShake.numero;
	
	char buffer[TAM_BUFFER];
	sprintf(buffer, "Cliente: %d Obtuvo vendedor: %d\n", nroCliente, nroVendedor);
	write(fileno(stdout), buffer, strlen(buffer));
}

ResultadoCompra ControladorCliente::comprarTicket(int dinero) {
	RequestTicketsMessage pedido;
	pedido.mtype = nroVendedor;
	pedido.origen = nroCliente;
	pedido.tipo = 0;
	pedido.ticketsInfo.cantidad = 1;
	pedido.ticketsInfo.dinero = dinero;
	comprarQueue.enviarPedido(pedido);
	
	ResponseTicketsMessage resultadoResponse;
	venderQueue.recibirResultado(nroCliente, &resultadoResponse);
	
	ResultadoCompra resultadoCompra = resultadoResponse.resultadoCompra;
	
	return resultadoCompra;
}

void  ControladorCliente::finalizarCompra() {
	
	//devolverCliente(host, nroCliente);
	
	// Le envio un mensaje al vendedor para indicarle que no compro mas tickets
	RequestTicketsMessage pedido;
	pedido.mtype = nroVendedor;
	pedido.origen = nroCliente;
	pedido.tipo = 1;
	pedido.ticketsInfo.cantidad = 0;
	pedido.ticketsInfo.dinero = 0;
	comprarQueue.enviarPedido(pedido);
}
