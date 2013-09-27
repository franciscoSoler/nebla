#include "../IPC/MessageQueue/ComprarMessageQueue.h"
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
		strcpy (buffer, "Canal Salida Vendedor: Error: Cantidad de parametros invalida.\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	sprintf (buffer,"Canal Salida Vendedor: Iniciando.\n");
	write(fileno(stdout), buffer, strlen(buffer)); 
	
	int socketSalida;
	sscanf(argv[1], "%d", &socketSalida);
	
	char nroClienteChar[TAM_BUFFER];
	int n = recibir(socketSalida, nroClienteChar, TAM_BUFFER);
	if (n < 0) {
		sprintf(buffer, "Canal Salida Vendedor - Fallo al recibir un cliente.\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	StartComunicationMessage startMsg;
	memcpy(&startMsg, nroClienteChar, sizeof(StartComunicationMessage));
	
	int nroCliente = startMsg.numero;
	
	sprintf(buffer, "Canal Salida Vendedor: Recibe un cliente: %d\n", nroCliente);
	write(fileno(stdout), buffer, strlen(buffer));
	
	VenderMessageQueue venderQueue = VenderMessageQueue();
	venderQueue.getMessageQueue((char*)DIRECTORY, ID_VENDER_1);

	bool deboSeguir = true;
	while (deboSeguir) {
		ResponseTicketsMessage resultadoResponse;		
		if (venderQueue.recibirResultado(nroCliente, &resultadoResponse) == -1) {
			sprintf(buffer, "Canal Salida Vendedor - Error al recibir un resultado. ERROR: %d.\n", errno);
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		} 

		NetworkSellMessage netMsg;
		netMsg.resultadoCompra = resultadoResponse.resultadoCompra;
		netMsg.origen = resultadoResponse.origen;
		netMsg.destino = nroCliente;
		netMsg.tipo = resultadoResponse.tipo;

		netMsg.size = sizeof(ResultadoCompra);
		memcpy(buffer, &netMsg, sizeof(NetworkSellMessage));
	
		if (enviar(socketSalida, buffer, TAM_BUFFER) != TAM_BUFFER) {
			sprintf(buffer, "Canal Salida Vendedor - Error al enviar un resultado\n");
			write (fileno(stderr),buffer, strlen(buffer));
			exit(-1);
		}

		if (resultadoResponse.tipo != 0) deboSeguir = false;
	}
	
	sprintf(buffer, "Canal Salida Vendedor - Finalizando.\n");
	write (fileno(stderr),buffer, strlen(buffer));
	
	close(socketSalida);
}

