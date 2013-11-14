
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"

#include "../Common.h"
#include "../Logger/Logger.h"
#include "LockFile.h"

extern int tcpopact(char *, int);

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
    
    char buffer[TAM_BUFFER];
    
    Logger::getInstance().setProcessInformation("Canal Salida Vendedor");
    
    if (argc != 2) {
        Logger::logMessage(Logger::ERROR, "Error: Cantidad de parametros invalida.");
        exit(1);
    }
      
    Logger::logMessage(Logger::TRACE, "Conectando canal de salida");
    
    int socketSalida;
    sscanf(argv[1], "%d", &socketSalida);
	
    /* Comienzo recibiendo el numero de cliente.
     * Es el mismo numero que luego voy a utilizar para leer de la cola.
     */
    char nroClienteChar[TAM_BUFFER];
    int n = recibir(socketSalida, nroClienteChar, TAM_BUFFER);
    if (n < 0) {
        Logger::logMessage(Logger::ERROR, "Fallo al recibir un cliente.");
        exit(-1);
    }

    StartComunicationMessage startMsg;
    memcpy(&startMsg, nroClienteChar, sizeof(StartComunicationMessage));	
    int nroCliente = startMsg.numero;

    sprintf(buffer, "Recibi un cliente: %d\n", nroCliente);
    Logger::logMessage(Logger::TRACE, buffer);
    
    try {
        IPC::ClientesMessageQueue clientesMsgQueue = IPC::ClientesMessageQueue("ClientesMessageQueue");
        clientesMsgQueue.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    
        bool deboSeguir = true;
        while (deboSeguir) {  
            
            msg_respuesta_pedido_t respuesta;
            clientesMsgQueue.recibirMensajeRespuesta(nroCliente, &respuesta);
            
            net_msg_respuesta_pedido_t netMsg;
	    netMsg.mensaje = respuesta;
            netMsg.destino = nroCliente;
            netMsg.tipo = respuesta.tipo;	
            netMsg.size = sizeof(net_msg_respuesta_pedido_t);
                	
            memcpy(buffer, &netMsg, sizeof(net_msg_respuesta_pedido_t));
		
            if (enviar(socketSalida, buffer, TAM_BUFFER) != TAM_BUFFER) {
		Logger::logMessage(Logger::ERROR, "Error al enviar un resultado.");
                exit(-1);
            }

	    if (respuesta.tipo != 0) deboSeguir = false;
	}
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        close(socketSalida);
        exit(-1);
    }

    Logger::logMessage(Logger::TRACE, "Finalizado.");
    close(socketSalida);    
}

