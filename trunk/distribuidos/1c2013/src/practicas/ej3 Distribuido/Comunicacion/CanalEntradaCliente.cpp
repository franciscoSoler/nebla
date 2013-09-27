#include <string.h>

#include "../IPC/MessageQueue/VenderMessageQueue.h"

#include "../common.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>

extern int tcpopact(char *, int);

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
	
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "Canal Entrada Cliente: Error: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
		
	int nroCliente = 0;
	sscanf(argv[1] , "%d", &nroCliente);
	
	sprintf(buffer, "Canal Entrada Cliente: Conectando canal de entrada para el cliente: %d\n", nroCliente);
	write(fileno(stdout), buffer, strlen(buffer));
	
	char server[TAM_BUFFER];
	int puertoEntrada = 0;
	int puertoSalida = 0;
	std::ifstream input ("Config", std::ifstream::in);
	if (! input) { 
		std::cerr << "Canal Entrada Cliente: Error al abrir el archivo\n"; 
		return -1; 
	}
	input >> server;
	input >> puertoEntrada;
	input >> puertoSalida;
	input.close();
	
	int socketEntrada = tcpopact(server, puertoSalida);

	sprintf(buffer, "Canal Entrada Cliente - Conectado al socket: %d\n", socketEntrada);
	write (fileno(stdout),buffer, strlen(buffer));
	
	VenderMessageQueue venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER_2);

	/* Envio mi numero de cliente */
	char nroClienteChar[TAM_BUFFER];
	StartComunicationMessage startMsg;
	startMsg.numero = nroCliente;
	memcpy(nroClienteChar, &startMsg, sizeof(StartComunicationMessage));
	enviar(socketEntrada, nroClienteChar, TAM_BUFFER);
	
	bool deboSeguir = true;
	while (deboSeguir) {
	
		char buffer[TAM_BUFFER];
		int n = recibir(socketEntrada,buffer,TAM_BUFFER);
		if (n < 0) {
			sprintf(buffer, "Canal Entrada Cliente - Cliente: %d Fallo al recibir un resultado\n", nroCliente);
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}
		
		NetworkSellMessage netMsg; 
		memcpy(&netMsg, buffer, sizeof(NetworkSellMessage));

		ResponseTicketsMessage resultadoResponse;
		resultadoResponse.resultadoCompra = netMsg.resultadoCompra;
		resultadoResponse.mtype = nroCliente; // o resultadoResponse.mtype = netMsg.destino
				
		if (netMsg.tipo != 0) deboSeguir = false;
		else venderQueue.enviarResultado(resultadoResponse);
	}	
	
	close(socketEntrada);
}
