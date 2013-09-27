
#include "../IPC/MessageQueue/GetVendedorMessageQueue.h"
#include "../IPC/MessageQueue/VenderMessageQueue.h"
#include "../IPC/MessageQueue/ComprarMessageQueue.h"

#include "../common.h"

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
	
	char buffer[TAM_BUFFER];

	if (argc != 2) {
		strcpy (buffer, "CanalComunicacionesEntrada Vendedor: Error: Cantidad de parametros invalida\n");
		write (fileno(stderr),buffer, strlen(buffer));
		exit(1);
	}
	
	sprintf (buffer,"Canal Entrada Vendedor: Iniciando\n");
	write(fileno(stdout), buffer, strlen(buffer)); 
	
	int socketEntrada = 0;
	sscanf(argv[1] , "%d", &socketEntrada);

	GetVendedorMessageQueue getVendedorQueue = GetVendedorMessageQueue();
	getVendedorQueue.getMessageQueue((char*)DIRECTORY, ID_GET_VENDEDOR_1);
	
	/* Optengo un vendedor libre */
	HandShakeMessage handShake;
	if (getVendedorQueue.recibirPedido(1,&handShake) < 0) {
		sprintf(buffer, "Canal Entrada Vendedor: Fallo al recibir un vendedor libre %d\n", errno);
		write(fileno(stderr), buffer, strlen(buffer));
		exit(1);
	}
	
	int nroVendedor = handShake.numero;
	sprintf(buffer, "Canal Entrada Vendedor: Recibe un vendedor libre %d\n", handShake.numero);
	write(fileno(stdout), buffer, strlen(buffer));
	
	ComprarMessageQueue comprarQueue = ComprarMessageQueue();
	comprarQueue.getMessageQueue((char*)DIRECTORY, ID_COMPRAR_1);

	/* Envio el numero de vendedor correspondiente */
	char nroVendedorChar[TAM_BUFFER];
	StartComunicationMessage startMsg;
	startMsg.numero = nroVendedor;
	memcpy(nroVendedorChar, &startMsg, sizeof(StartComunicationMessage));
	enviar(socketEntrada, nroVendedorChar, TAM_BUFFER);
		
	sprintf(buffer, "Canal Entrada Vendedor: Enviando vendedor %d por el socket %d\n", nroVendedor, socketEntrada);
	write(fileno(stdout), buffer, strlen(buffer));
	
	if (enviar(socketEntrada, nroVendedorChar, TAM_BUFFER) != TAM_BUFFER) {
		sprintf(buffer, "Canal Entrada Vendedor - Vendedor: %d Error al enviar el numero de vendedor.\n",nroVendedor);
		write (fileno(stderr),buffer, strlen(buffer));
		exit(-1);
	}
	
	bool deboSeguir = true;
	while (deboSeguir) {
	
		char buffer[TAM_BUFFER];
		NetworkBuyMessage netMsg;
		RequestTicketsMessage ticketsInfoMsg;
		
		if (recibir(socketEntrada, buffer, TAM_BUFFER) != TAM_BUFFER) {
			sprintf(buffer, "Canal Entrada Vendedor - Error al recibir un pedido de ticket. ERROR: %d.\n", errno);
			write (fileno(stderr),buffer, strlen(buffer));
			close(socketEntrada);
			exit(-1);
		}
		
		memcpy(&netMsg, buffer, sizeof(NetworkBuyMessage));
		
		sprintf(buffer, "Canal Entrada Vendedor - Vendedor: %d Recibió un pedido.\
													Destino: %d\
													Origen: %d\
													Cantidad: %d\
													Dinero: %f\
													Tipo: %d.\n",
													nroVendedor, netMsg.destino, netMsg.origen, netMsg.ticketsInfo.cantidad, netMsg.ticketsInfo.dinero, netMsg.tipo);
		write (fileno(stderr),buffer, strlen(buffer));
		
		ticketsInfoMsg.mtype = nroVendedor; // o ticketsInfoMsg.mtype = netMsg.destino
		ticketsInfoMsg.origen = netMsg.origen;
		ticketsInfoMsg.ticketsInfo = netMsg.ticketsInfo;
		
		comprarQueue.enviarPedido(ticketsInfoMsg);
		
		if (netMsg.tipo != 0) {
			deboSeguir = false;
			sprintf(buffer, "Canal Entrada Vendedor - El cliente finalizó la compra.\n");
			close(socketEntrada);
			write (fileno(stderr),buffer, strlen(buffer));
		}
	}
				
	close(socketEntrada);
	
}
