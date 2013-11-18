#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <string.h>

#include "../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"

#include "../Common.h"
#include "../Logger/Logger.h"
#include "LockFile.h"

extern int tcpopact(char *, int);

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {

    char buffer[TAM_BUFFER];

    if (argc != 2) {
        Logger::getInstance().setProcessInformation("Canal Salida Cliente");
        Logger::logMessage(Logger::ERROR, "Error: Cantidad de parametros invalida.");
        exit(1);
    }
  
    int nroCliente = 0;
    sscanf(argv[1], "%d", &nroCliente);

    sprintf(buffer, "Canal Salida Cliente %d", nroCliente);
    Logger::getInstance().setProcessInformation(buffer);
    
    Logger::logMessage(Logger::COMM, "Conectando canal de salida");

    char server[TAM_BUFFER];
    int puertoEntrada = 0;
    std::ifstream input("Config", std::ifstream::in);
    if (!input) {
        Logger::logMessage(Logger::ERROR, "Error al abrir el archivo de configuracion.");
        exit(1);
    }
    input >> server;
    input >> puertoEntrada;
    input.close();

    // Me conecto al puerto de Entrada del servidor.
    int socketSalida = tcpopact(server, puertoEntrada);

    sprintf(buffer, "Conectado al socket: %d", socketSalida);
    Logger::logMessage(Logger::COMM, buffer);
	
    // Recibo el numero de vendedor
    char nroVendedorChar[TAM_BUFFER];
    int n = recibir(socketSalida, nroVendedorChar, TAM_BUFFER);	
    if (n < 0) {
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
            sprintf(buffer, "Recibió un pedido. Mtype: %ld", mensajePedido.mtype);		
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
	
            if (enviar(socketSalida, buffer, TAM_BUFFER) != TAM_BUFFER) {
                Logger::logMessage(Logger::ERROR, "Error al enviar un pedido,");
                close(socketSalida);
                exit(-1);
            }
        }
	
        Logger::logMessage(Logger::COMM, "Finalizando.");
        close(socketSalida);
    }   
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        exit(-1);
    }
}