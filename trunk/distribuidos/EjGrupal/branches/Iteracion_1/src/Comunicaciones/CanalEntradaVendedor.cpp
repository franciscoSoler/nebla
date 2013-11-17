
#include "../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../IPCs/IPCAbstractos/MessageQueue/VendedoresMessageQueue.h"

#include "../Common.h"
#include "Logger.h"
#include "LockFile.h"
#include "PedidosVendedorMessageQueue.h"

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
    
    char buffer[TAM_BUFFER];
        
    Logger::getInstance().setProcessInformation("Canal Entrada Vendedor");
    
    if (argc != 2) {
        Logger::logMessage(Logger::ERROR, "Error: Cantidad de parametros invalida.");
        exit(1);
    }
    
    Logger::logMessage(Logger::TRACE, "Conectando canal de entrada");
    
    int socketEntrada = 0;
    sscanf(argv[1] , "%d", &socketEntrada);

    try {
        IPC::VendedoresMessageQueue getVendedorQueue = IPC::VendedoresMessageQueue("VendedoresMessageQueue");
        getVendedorQueue.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
	
        /* Optengo un vendedor libre */
        mensaje_inicial_t handShake; 
        getVendedorQueue.recibirMensajeInicial(TIPO_VENDEDOR_LIBRE,&handShake);
		
        int nroVendedor = handShake.emisor;
    
        sprintf(buffer, "Canal Entrada Vendedor %d",nroVendedor);
        Logger::getInstance().setProcessInformation(buffer);

        Logger::logMessage(Logger::TRACE,"Recibe un vendedor libre");
        
        IPC::PedidosVendedorMessageQueue pedidos = IPC::PedidosVendedorMessageQueue("PedidosMsgQueue");
        pedidos.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
	
	/* Envio el numero de vendedor correspondiente */
	char nroVendedorChar[TAM_BUFFER];
	StartComunicationMessage startMsg;
	startMsg.numero = nroVendedor;
	memcpy(nroVendedorChar, &startMsg, sizeof(StartComunicationMessage));
	enviar(socketEntrada, nroVendedorChar, TAM_BUFFER);
		
	sprintf(buffer, "Enviando vendedor %d por el socket %d\n", nroVendedor, socketEntrada);
        Logger::logMessage(Logger::TRACE, buffer);
	
	if (enviar(socketEntrada, nroVendedorChar, TAM_BUFFER) != TAM_BUFFER) {
            Logger::logMessage(Logger::ERROR,"Error al enviar el numero de vendedor.");
            exit(-1);
	}
	
	bool deboSeguir = true;
	while (deboSeguir) {

            net_msg_pedido_t netMsg;
            msg_pedido_t pedido;
		
            if (recibir(socketEntrada, buffer, TAM_BUFFER) != TAM_BUFFER) {
                sprintf(buffer, "Error al recibir un pedido de ticket. ERROR: %d.\n", errno);
                Logger::logMessage(Logger::ERROR, buffer);
		close(socketEntrada);
		exit(-1);
            }

            memcpy(&netMsg, buffer, sizeof(net_msg_pedido_t));
	   
            sprintf(buffer, "Recibió un pedido. Destino: %d Origen: %d Tipo: %d.", netMsg.destino, netMsg.origen, netMsg.tipo);
            Logger::logMessage(Logger::TRACE, buffer);
		
            pedido = netMsg.mensaje;
	   
            pedidos.enviarMensajePedido(pedido);
		
            if (netMsg.tipo != 0) {
		deboSeguir = false;
                Logger::logMessage(Logger::TRACE,"El cliente finalizó la compra.");
            }
	}
				
	close(socketEntrada);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}
