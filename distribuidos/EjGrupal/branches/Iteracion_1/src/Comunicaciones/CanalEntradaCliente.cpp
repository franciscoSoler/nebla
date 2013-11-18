#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "../IPCs/IPCAbstractos/MessageQueue/PedidosVendedorMessageQueue.h"
#include "../Logger/Logger.h"
#include "../Common.h"
#include "ClientesMessageQueue.h"

extern int tcpopact(char *, int);

extern int enviar(int sockfd, void *datos, size_t nbytes);
extern int recibir(int sockfd, void *datos, size_t nbytes);

int main(int argc, char **argv) {
    char buffer[TAM_BUFFER];

    if (argc != 2) {
        Logger::getInstance().setProcessInformation("Canal Entrada Cliente");
        Logger::logMessage(Logger::ERROR, "Error: Cantidad de parametros invalida.");
        exit(1);
    }

    int nroCliente = 0;
    sscanf(argv[1], "%d", &nroCliente);

    sprintf(buffer, "Canal Entrada Cliente %d", nroCliente);
    Logger::getInstance().setProcessInformation(buffer);
    Logger::logMessage(Logger::COMM, "Conectando canal de entrada");

    char server[TAM_BUFFER];
    int puertoEntrada = 0;
    int puertoSalida = 0;
    std::ifstream input("Config", std::ifstream::in);
    if (!input) {
        Logger::logMessage(Logger::ERROR, "Error al abrir archivo de configuración");
        exit(1);
    }
    input >> server;
    input >> puertoEntrada;
    input >> puertoSalida;
    input.close();

    int socketEntrada = tcpopact(server, puertoSalida);
    sprintf(buffer, "Conectado al socket: %d", socketEntrada);
    Logger::logMessage(Logger::COMM, buffer);

    IPC::ClientesMessageQueue clientesMessageQueue = IPC::ClientesMessageQueue("Cliente Msg Queue");
    clientesMessageQueue.getMessageQueue((char*) DIRECTORY_VENDEDOR, ID_COLA_CLIENTES_C);

    /* Envio mi numero de cliente */
    char nroClienteChar[TAM_BUFFER];
    StartComunicationMessage startMsg;
    startMsg.numero = nroCliente;
    memcpy(nroClienteChar, &startMsg, sizeof (StartComunicationMessage));
    enviar(socketEntrada, nroClienteChar, TAM_BUFFER);

    bool deboSeguir = true;
    while (deboSeguir) {

        char buffer[TAM_BUFFER];
        int n = recibir(socketEntrada, buffer, TAM_BUFFER);
        if (n < 0) {
            Logger::logMessage(Logger::ERROR, "Fallo al recibir un resultado");
            close(socketEntrada);
            exit(-1);
        }

        net_msg_respuesta_pedido_t netMsg;
        memcpy(&netMsg, buffer, sizeof (net_msg_respuesta_pedido_t));

        msg_respuesta_pedido_t resultadoRespuesta;
        resultadoRespuesta = netMsg.mensaje;

        if (netMsg.tipo != 0) deboSeguir = false;
        else clientesMessageQueue.enviarMensajeRespuesta(resultadoRespuesta);
    }

    close(socketEntrada);
}
