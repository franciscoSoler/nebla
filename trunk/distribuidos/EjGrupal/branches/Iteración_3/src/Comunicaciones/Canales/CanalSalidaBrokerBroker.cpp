#include <Common.h>
#include <stdlib.h>

#include <Logger/Logger.h>
#include <Socket/SocketConnector.h>

#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Comunicaciones/Objects/ServersManager.h>

int main(int argc, char* argv[]) {
    ArgumentParser argParser(argc, argv);
    char buffer[TAM_BUFFER];
    char bufferSocket[TAM_BUFFER];

    int socketDescriptor = 0;
    int brokerNumber = 0;
    int remoteBrokerId = 0;

    if ( argParser.parseArgument(1, socketDescriptor) == -1 ) {
        Logger::logMessage(Logger::ERROR, "ERROR: Parse Argument 1.");
        exit(-1);
    }

    if ( argParser.parseArgument(2, brokerNumber) == -1 ) {
        Logger::logMessage(Logger::ERROR, "ERROR: Parse Argument 2.");
        exit(-1);
    }

    if ( argc == 4 && socketDescriptor == 0 ) {
        if ( argParser.parseArgument(3, remoteBrokerId) == -1 ) {
            Logger::logMessage(Logger::ERROR, "ERROR: Parse Argument 3.");
            exit(-1);
        }
    }

    sprintf(buffer, "CanalSalidaBrokerBroker N°%d - N°%d:", brokerNumber, remoteBrokerId);
    Logger::setProcessInformation(buffer);

    SocketStream* socketBroker = NULL;
    if ( socketDescriptor == 0 ) {
        sprintf(buffer, "Creando conexión con Broker N°%d", remoteBrokerId);
        Logger::logMessage(Logger::DEBUG, buffer);
        // El proceso no fue creado por un servidor, debe conectarse al
        // mismo
        ServersManager serversManager;
        SocketStream::SocketStreamPtr ptrSocketBroker(
        serversManager.connectToBrokerServer("ServidorCanalEntradaBrokerBroker",
                                             remoteBrokerId) );

        if ( ptrSocketBroker.get() == NULL ) {
            abort();
        }
        socketBroker = ptrSocketBroker.get();

        // Envía al otro extremo del canal su número de Broker
        memcpy(bufferSocket, &brokerNumber, sizeof(int));
        if ( socketBroker->send(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
            Logger::logMessage(Logger::ERROR, "Error al envíar brokerNumber por el Socket");
            socketBroker->destroy();
            abort();
        }
    }
    else {
        // El proceso fue creado por un servidor
        SocketStream aux( socketDescriptor );
        socketBroker = &aux;

        // Recibe el número de Broker con el cual se conectó
        if ( socketBroker->receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
            Logger::logMessage(Logger::ERROR, "Error al recibir brokerNumber por el Socket");
            socketBroker->destroy();
            abort();
        }
        memcpy(& remoteBrokerId, bufferSocket, sizeof(int));
    }

    sprintf(buffer, "CanalSalidaBrokerBroker N°%d - N°%d:", brokerNumber, remoteBrokerId);
    Logger::setProcessInformation(buffer);
    Logger::logMessage(Logger::COMM, "Conexión realizada correctamente");

    // TODO: Insert code here
    Logger::logMessage(Logger::COMM, "Destruyendo canal");
    socketBroker->destroy();
    return 0;
}
