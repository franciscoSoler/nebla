#include <Common.h>
#include <stdlib.h>

#include <Logger/Logger.h>
#include <Socket/SocketConnector.h>

#include <Common.h>
#include <middlewareCommon.h>

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/SharedMemory.h>

#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Comunicaciones/Objects/ServersManager.h>

#include <Util.h>

#include <sys/types.h>
#include <sys/wait.h>

static const char* C_DIRECTORY_BROKER = NULL;

void elegirDirectorios(int brokerNumber);

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

    elegirDirectorios( brokerNumber );

    sprintf(buffer, "CanalSalidaBrokerBroker N°%d - N°%d:", brokerNumber, remoteBrokerId);
    Logger::setProcessInformation(buffer);

    IPC::MsgQueue msgQueueACK;
    msgQueueACK.getMsgQueue(C_DIRECTORY_BROKER, ID_MSGQUEUE_TIMEOUT);

    SocketStream* socketBroker = NULL;
    if ( socketDescriptor == 0 ) {
        sprintf(buffer, "Creando conexión con Broker N°%d", remoteBrokerId);
        Logger::logMessage(Logger::DEBUG, buffer);
        // El proceso no fue creado por un servidor, debe conectarse al
        // mismo
        ServersManager serversManager;
        socketBroker = serversManager.connectToBrokerServer(
                "ServidorCanalEntradaBrokerBroker", remoteBrokerId);

        if ( socketBroker == NULL ) {
            abort();
        }

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


    try {
        IPC::MsgQueue colaCanalSalida("ColaCanalSalida");
        colaCanalSalida.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);

        ulong idMensajeBrokerBrokerEnviado = 1;

        while ( true ) {
            MsgCanalSalidaBrokerBroker mensaje;
            colaCanalSalida.recv(remoteBrokerId, mensaje);

            if(mensaje.msg.tipoMensaje == MENSAJE_ACK)
            {
                sprintf(buffer, "Quiere enviar un mensaje ACK %lu (TIMEOUT).", mensaje.msg.msg_id);
                Logger::logMessage(Logger::DEBUG, buffer);
            }

            else if(mensaje.msg.tipoMensaje == MEMORIA_AGENTE)
            {
                sprintf(buffer, "Recibe mensaje, procede a enviarlo como mensaje %lu. (TIMEOUT B%d)", idMensajeBrokerBrokerEnviado, brokerNumber);
                Logger::logMessage(Logger::DEBUG, buffer);

                char idMensajeBrokerBrokerEnviado_str[128];
                char idBroker_str[32];
                char idBrokerRemoto_str[32];

                sprintf(idMensajeBrokerBrokerEnviado_str, "%lu", idMensajeBrokerBrokerEnviado);
                sprintf(idBroker_str, "%d", brokerNumber);
                sprintf(idBrokerRemoto_str, "%d", remoteBrokerId);

                Util::safeCreateProcess("Timeout", std::string(idBroker_str), std::string(idBrokerRemoto_str), std::string(idMensajeBrokerBrokerEnviado_str));

                mensaje.msg.msg_id = idMensajeBrokerBrokerEnviado;
            }

            memcpy(bufferSocket, &mensaje.msg, sizeof(MsgCanalEntradaBrokerBroker));
            if ( socketBroker->send(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al enviar mensaje a Broker");
                socketBroker->destroy();
                abort();
            }

            if(mensaje.msg.tipoMensaje == MEMORIA_AGENTE)
            {
                MsgACK ack;
                sprintf(buffer, "Canal va a esperar en la cola %d (TIMEOUT)", remoteBrokerId);
                Logger::logMessage(Logger::DEBUG, buffer);
                msgQueueACK.recv(remoteBrokerId, ack);

                if(ack.msg_id != idMensajeBrokerBrokerEnviado)
                {
                    sprintf(buffer, "ERROR: recibí ACK con id %lu en vez de %lu. (TIMEOUT)", ack.msg_id, idMensajeBrokerBrokerEnviado);
                    Logger::logMessage(Logger::ERROR, buffer);
                }

                else if(ack.recepcionOK == true)
                    Logger::logMessage(Logger::DEBUG, "Recibido ACK correcto. (TIMEOUT)");

                else
                    Logger::logMessage(Logger::ERROR, "Se cayó el broker siguiente!! (TIMEOUT)");


                idMensajeBrokerBrokerEnviado++;

            }

        }
    }
    catch( Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        socketBroker->destroy();
        if (socketDescriptor == 0) {
            delete socketBroker;
        }
        abort();
    }

    Logger::logMessage(Logger::COMM, "Destruyendo canal");
    socketBroker->destroy();
    if (socketDescriptor == 0) {
        delete socketBroker;
    }
    return 0;
}

void elegirDirectorios(int brokerNumber) {
    switch (brokerNumber) {
        case 1:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_1;
            // C_DIRECTORY_ADM = DIRECTORY_ADM_1;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_1;
            break;
        case 2:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_2;
            // C_DIRECTORY_ADM = DIRECTORY_ADM_2;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_2;
            break;
        case 3:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_3;
            // C_DIRECTORY_ADM = DIRECTORY_ADM_3;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_3;
            break;
        case 4:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_4;
            // C_DIRECTORY_ADM = DIRECTORY_ADM_4;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_4;
            break;
        default:
            Logger::logMessage(Logger::ERROR, "Error al elegir directorios del Broker");
            abort();
    }
}
