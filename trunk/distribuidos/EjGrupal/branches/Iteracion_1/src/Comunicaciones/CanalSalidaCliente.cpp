#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <assert.h>

#include "../Common.h"
#include "../Logger/Logger.h"
#include "../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"

#include <Comunicaciones//Objects/CommunicationsUtil.h>
#include <Socket/SocketConnector.h>
#include <Socket/SocketStream.h>

extern int tcpopact(char *, int);

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
    CommunicationsUtil util;
    char buffer[TAM_BUFFER];
    int nroCliente = 0;

    if ( util.parseArgs(argc, argv, nroCliente) == -1 ) {
        exit(-1);
    }

    sprintf(buffer, "Canal Salida Cliente %d:", nroCliente);
    Logger::getInstance().setProcessInformation(buffer);
    Logger::logMessage(Logger::COMM, "Conectando canal de salida");

    char server[TAM_BUFFER];
    int puertoEntrada = 0;
    int puertoSalida = 0; // Unused
    if ( util.parseChannelArgs(server, puertoEntrada, puertoSalida) == -1 ) {
        exit(-1);
    }

    // Me conecto al puerto de Entrada del servidor.
    SocketConnector connector;
    SocketStream::SocketStreamPtr socketSalida( connector.connect(puertoEntrada, server) );
    assert( socketSalida.get() );

    sprintf(buffer, "Conectado al socket: %d", socketSalida->getSd());
    Logger::logMessage(Logger::COMM, buffer);
	
    // Recibo el numero de vendedor
    char nroVendedorChar[TAM_BUFFER];
    int n = socketSalida->receive(nroVendedorChar, TAM_BUFFER);
    if (n <= 0) {
        Logger::logMessage(Logger::ERROR,"Fallo al recibir un vendedor.");
        exit(-1);
    }
   
    StartComunicationMessage startMsg;	
    memcpy(&startMsg, nroVendedorChar, sizeof(StartComunicationMessage));
    int nroVendedor = startMsg.numero;
    sprintf(buffer, "Obtuvo vendedor: %d", nroVendedor);
    Logger::logMessage(Logger::COMM,buffer);
	
    try {
        // Le envio el nro vendedor por la cola al cliente	
        msg_respuesta_pedido_t mensajeRespuesta;
        mensajeRespuesta.mtype = nroCliente;
        respuesta_pedido_t respuesta;
        respuesta.emisor = nroVendedor;
        mensajeRespuesta.respuesta_pedido = respuesta;

        IPC::ClientesMessageQueue clientes = IPC::ClientesMessageQueue("ClientesMessageQueue");
        clientes.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_CLIENTES_C);
        clientes.enviarMensajeRespuesta(mensajeRespuesta);
    
        IPC::PedidosVendedorMessageQueue pedidosMessageQueue = IPC::PedidosVendedorMessageQueue("Pedidos Vendedor MessageQueue");
        pedidosMessageQueue.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS_C);
    
        bool deboSeguir = true;
        while (deboSeguir) {
            msg_pedido_t mensajePedido;
            pedidosMessageQueue.recibirMensajePedido(nroVendedor, &mensajePedido);
            sprintf(buffer, "Recibió un pedido. Mtype: %ld - Tipo: %d", mensajePedido.mtype, mensajePedido.tipo);
            Logger::logMessage(Logger::COMM, buffer);
                
            net_msg_pedido_t netMsg;
            netMsg.mensaje = mensajePedido;
            netMsg.tipo = mensajePedido.tipo;
            netMsg.origen = nroCliente;
            netMsg.destino = nroVendedor;
            netMsg.size = sizeof(net_msg_pedido_t);
		    
            if (mensajePedido.tipo != 0) {
                deboSeguir = false;
            }
	   
            memcpy(buffer, &netMsg, sizeof(net_msg_pedido_t));
	
            if (socketSalida->send(buffer, TAM_BUFFER) != TAM_BUFFER) {
                Logger::logMessage(Logger::ERROR, "Error al enviar un pedido,");
                socketSalida->destroy();
                exit(-1);
            }
        }
	
        Logger::logMessage(Logger::COMM, "Finalizando.");
        socketSalida->destroy();
    }   
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        exit(-1);
    }
}
