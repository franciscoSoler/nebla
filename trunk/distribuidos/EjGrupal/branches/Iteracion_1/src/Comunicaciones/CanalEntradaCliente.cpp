#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

#include "../IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"
#include "../Logger/Logger.h"
#include "../Common.h"
#include "ClientesMessageQueue.h"

#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Objects/ServersManager.h>
#include <Socket/SocketStream.h>


int main(int argc, char **argv) {
    CommunicationsUtil util;
    char buffer[255];
    long nroCliente;

    if ( util.parseArgs(argc, argv, nroCliente) == -1 ) {
        exit(1);
    }

    sprintf(buffer, "Canal Entrada Cliente %ld:", nroCliente);
    Logger::getInstance().setProcessInformation(buffer);
    Logger::logMessage(Logger::COMM, "Conectando canal de entrada");

    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketEntrada(
    serversManager.connectToServer("ServidorVendedorSalida") );
    assert( socketEntrada.get() );

    sprintf(buffer, "Conectado al socket: %d", socketEntrada->getSd());
    Logger::logMessage(Logger::COMM, buffer);

    IPC::ClientesMessageQueue clientesMessageQueue = IPC::ClientesMessageQueue("Cliente Msg Queue");
    clientesMessageQueue.getMessageQueue((char*) DIRECTORY_VENDEDOR, ID_COLA_CLIENTES_C);

    /* Envio mi numero de cliente */
    char nroClienteChar[TAM_BUFFER];
    StartComunicationMessage startMsg;
    startMsg.numero = nroCliente;
    memcpy(nroClienteChar, &startMsg, sizeof (StartComunicationMessage));
    socketEntrada->send(nroClienteChar, TAM_BUFFER);

    bool deboSeguir = true;
    while (deboSeguir) {

        char buffer[TAM_BUFFER];
        int n = socketEntrada->receive(buffer, TAM_BUFFER);
        if (n <= 0) {
            Logger::logMessage(Logger::ERROR, "Fallo al recibir un resultado");
            socketEntrada->destroy();
            exit(-1);
        }

        net_msg_respuesta_pedido_t netMsg;
        memcpy(&netMsg, buffer, sizeof (net_msg_respuesta_pedido_t));

        msg_respuesta_pedido_t resultadoRespuesta;
        resultadoRespuesta = netMsg.mensaje;

        if (netMsg.tipo != 0) deboSeguir = false;
        else clientesMessageQueue.enviarMensajeRespuesta(resultadoRespuesta);
    }

    socketEntrada->destroy();
}
