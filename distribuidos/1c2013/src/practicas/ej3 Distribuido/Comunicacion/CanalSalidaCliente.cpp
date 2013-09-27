#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <string.h>

#include "../IPC/MessageQueue/ComprarMessageQueue.h"
#include "../IPC/MessageQueue/VenderMessageQueue.h"

#include "../common.h"

extern int tcpopact(char *, int);

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
		
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Canal Salida Cliente: Error: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	int nroCliente = 0;
	sscanf(argv[1] , "%d", &nroCliente);
	
	sprintf(buffer, "Canal Salida Cliente: Conectando canal de salida para el cliente: %d\n", nroCliente);
	write (fileno(stdout),buffer, strlen(buffer));
	
	char server[TAM_BUFFER];
	int puertoEntrada = 0;
	std::ifstream input ("Config", std::ifstream::in);
	if (! input) { 
		std::cerr << "Canal Salida Cliente: Error al abrir el archivo de Configuracion\n"; 
		return -1; 
	}
	input >> server;
	input >> puertoEntrada;
	input.close();

	// Me conecto al puerto de Entrada del servidor.
	int socketSalida = tcpopact(server, puertoEntrada);
	
	sprintf(buffer, "Canal Salida Cliente - Conectado al socket: %d\n", socketSalida);
	write (fileno(stdout),buffer, strlen(buffer));

	ComprarMessageQueue comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR_2);
	
	char nroVendedorChar[TAM_BUFFER];
	int n = recibir(socketSalida, nroVendedorChar, TAM_BUFFER);
	if (n < 0) {
		sprintf(buffer, "Canal Salida Cliente - Cliente: %d Fallo al recibir un vendedor\n", nroCliente);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	StartComunicationMessage startMsg;
	memcpy(&startMsg, nroVendedorChar, sizeof(StartComunicationMessage));
			
	int nroVendedor = startMsg.numero;
		
	sprintf(buffer, "Canal Salida Cliente - Cliente: %d obtuvo vendedor: %d\n", nroCliente, nroVendedor);
	write (fileno(stdout),buffer, strlen(buffer));
	
	HandShakeMessage message;
	message.numero = nroVendedor;
	message.mtype = nroCliente;
	
	VenderMessageQueue venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER_2);
	venderQueue.enviarNroVendedor(message);
	
	bool deboSeguir = true;
	while (deboSeguir) {
		RequestTicketsMessage request;
		if (comprarQueue.recibirPedido(nroVendedor,&request) == -1) {
			sprintf(buffer, "Canal Salida Cliente - Cliente: %d Error al recibir un pedido, ERROR: %d\n", nroCliente, errno);
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
		else {
			sprintf(buffer, "Canal Salida Cliente - Cliente: %d Recibió un pedido. Mtype: %ld Origen: %d Cantidad: %d Dinero: %f Tipo: %d \n", 
					nroCliente, request.mtype, request.origen, request.ticketsInfo.cantidad, request.ticketsInfo.dinero, request.tipo);
			write (fileno(stderr),buffer, strlen(buffer));
		}
	
		NetworkBuyMessage netMsg;
		netMsg.ticketsInfo = request.ticketsInfo;
		netMsg.tipo = request.tipo;
		netMsg.origen = nroCliente;
		netMsg.destino = nroVendedor;
		netMsg.size = sizeof(TicketsInfo);
		
		if (request.tipo != 0) {
			deboSeguir = false;
		}
		
		char buffer[TAM_BUFFER];
		memcpy(buffer, &netMsg, sizeof(NetworkBuyMessage));
		
		if (enviar(socketSalida, buffer, TAM_BUFFER) != TAM_BUFFER) {
			sprintf(buffer, "Canal Salida Cliente - Cliente: %d Error al enviar un pedido\n", nroCliente);
			write (fileno(stderr),buffer, strlen(buffer));
			close(socketSalida);
			exit(-1);
		}
	}
	
	sprintf(buffer, "Canal Salida Cliente - Cliente: %d Finalizando.\n",nroCliente);
	write (fileno(stderr),buffer, strlen(buffer));
	
	close(socketSalida);
}

