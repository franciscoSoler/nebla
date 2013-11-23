#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "../Common.h"
#include "../Logger/Logger.h"

#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Signals/SignalHandler.h>
#include <Comunicaciones/Signals/Handlers/SIGINT_Handler.h>
#include <Socket/SocketAcceptor.h>
#include <Socket/SocketStream.h>

int main(int argc, char **argv) {
    Logger::setProcessInformation("Servidor Vendedor Salida:");
    /* string que contiene el socket para el servidor de eco */
    static char socketSalidaChar[15];
    CommunicationsUtil util;
    char buffer[TAM_BUFFER];

    SIGINT_Handler handler;
    SignalHandler::getInstance().registerHandler(SIGINT, &handler);

    char server[TAM_BUFFER]; // Unused
    int puertoEntrada = 0;
    int puertoSalida = 0;
    if ( util.parseChannelArgs(server, puertoEntrada, puertoSalida) == -1 ) {
        exit(-1);
    }

    sprintf(buffer, "Levantando server: %s en puerto %d.", server, puertoSalida);
    Logger::logMessage(Logger::COMM, buffer);

    SocketAcceptor acceptor(puertoSalida);
    acceptor.configureSocket();
    Logger::logMessage(Logger::COMM, "Comienza a escuchar conexiones de clientes");

    while ( true ) {
        SocketStream::SocketStreamPtr socketSalida( acceptor.accept() );
        if (not socketSalida.get() ) {
            break;
        }
        Logger::logMessage(Logger::COMM, "Acepta conexión. Procede a forkear la misma");
        sprintf(socketSalidaChar, "%d", socketSalida->getSd());

        pid_t pid = fork();
        if (pid == 0) {
            execlp("./CanalSalidaVendedor", "CanalSalidaVendedor", socketSalidaChar, (char *) 0);
            sprintf(buffer, "execlp ./CanalSalidaVendedor error: %s", strerror(errno));
            Logger::logMessage(Logger::ERROR, buffer);
        }
        // No se si esto está bien, por el momento funciona (en OS Debian Wheezy - 64 bits)
        socketSalida->destroy();
    }

    acceptor.destroy();
    return 0;
}

