#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../IPCs/IPCAbstractos/MessageQueue/ClientesMessageQueue.h"
#include "../Common.h"
#include "../Logger/Logger.h"
#include "LockFile.h"

#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Socket/SocketStream.h>

int main(int argc, char **argv) {
    char buffer[TAM_BUFFER];
    Logger::getInstance().setProcessInformation("Canal Salida Vendedor:");
    int sd;
    CommunicationsUtil util;

    if ( util.parseArgs(argc, argv, sd) == -1) {
        exit(-1);
    }
    
    Logger::logMessage(Logger::COMM, "Conectando canal de salida");
    SocketStream socketSalida(sd);
	
    /* Comienzo recibiendo el numero de cliente.
     * Es el mismo numero que luego voy a utilizar para leer de la cola.
     */
    char nroClienteChar[TAM_BUFFER];
    if ( socketSalida.receive(nroClienteChar, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Fallo al recibir un cliente.");
        exit(-1);
    }

    StartComunicationMessage startMsg;
    memcpy(&startMsg, nroClienteChar, sizeof(StartComunicationMessage));	
    int nroCliente = startMsg.numero;
    sprintf(buffer, "Recibi un cliente: %d", nroCliente);
    Logger::logMessage(Logger::COMM, buffer);
    
    try {
        IPC::ClientesMessageQueue clientesMsgQueue = IPC::ClientesMessageQueue("ClientesMessageQueue");
        clientesMsgQueue.getMessageQueue((char*)DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    
        bool deboSeguir = true;
        while (deboSeguir) {  
            
            msg_respuesta_pedido_t respuesta;
            clientesMsgQueue.recibirMensajeRespuesta(nroCliente, &respuesta);

            sprintf(buffer, "mtype: %ld - recOK: %d - tipo: %d",
            respuesta.mtype, respuesta.respuesta_pedido.recepcionOK, respuesta.tipo );
            Logger::logMessage(Logger::DEBUG, buffer);
            
            net_msg_respuesta_pedido_t netMsg;
            netMsg.mensaje = respuesta;
            netMsg.destino = nroCliente;
            netMsg.tipo = respuesta.tipo;	
            netMsg.size = sizeof(net_msg_respuesta_pedido_t);
                	
            memcpy(buffer, &netMsg, sizeof(net_msg_respuesta_pedido_t));
		
            if ( socketSalida.send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar un resultado.");
                exit(-1);
            }

            if (respuesta.tipo != 0) deboSeguir = false;
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        socketSalida.destroy();
        exit(-1);
    }

    Logger::logMessage(Logger::COMM, "Finalizado.");
    socketSalida.destroy();
}

