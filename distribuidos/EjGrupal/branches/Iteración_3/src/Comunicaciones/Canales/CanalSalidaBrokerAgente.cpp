#include <iostream>
#include <string.h>
#include <assert.h>
#include <sstream>

#include <Logger/Logger.h>
#include <Common.h>
#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Socket/SocketStream.h>

#include "middlewareCommon.h"

int main(int argc, char* argv[]) {
    char buffer[TAM_BUFFER];
    char bufferSocket[TAM_BUFFER];
    char bufferMsgQueue[MSG_BROKER_SIZE];
    ArgumentParser argParser(argc, argv);
    int idSd = 0;
    long idAgente = 0;
    int brokerNumber = 0;
    int idTipoAgente;

    if ( argParser.parseArgument(1, idSd) == -1 ) {
        exit(-1);
    }

    if ( argParser.parseArgument(2, brokerNumber) == -1 ) {
        exit(-1);
    }

    sprintf(buffer, "CanalEntradaBrokerAgente N°%d", brokerNumber);
    Logger::setProcessInformation( buffer );

    SocketStream socketAgente(idSd);
    Logger::logMessage(Logger::COMM, "Canal de Salida conectado");
    
    // Recibe el id los datos del Agente
    if ( socketAgente.receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al recibir el id del Agente");
        socketAgente.destroy();
        abort();
    }
    
    std::stringstream ss;
    ss << bufferSocket;
    ss >> idAgente;
    ss >> idTipoAgente;
    
    sprintf(buffer, "Se reciben los datos del agente: %ld - %d",
            idAgente, idTipoAgente);
    Logger::logMessage(Logger::COMM, buffer);
    
    try {
        IPC::MsgQueue colaBroker("cola Broker");
        colaBroker.getMsgQueue(DIRECTORY_BROKER, idTipoAgente);
        
        while ( true ) {
            MsgCanalSalidaBroker mensaje;
            //colaBroker.recv(idAgente, mensaje);
            colaBroker.recv(idAgente, bufferMsgQueue, MSG_BROKER_SIZE);
            memcpy(&mensaje, bufferMsgQueue, sizeof(MsgCanalSalidaBroker));
            
            sprintf(buffer, "Recibe mensaje de Broker, con tamaño %d", sizeof(mensaje));
            Logger::logMessage(Logger::COMM, buffer);
            
            sprintf(buffer, "parametros: mtype recibido (idAgente): %ld", 
            mensaje.mtype);
            Logger::logMessage(Logger::COMM, buffer);
            
            
            memcpy(bufferSocket, &mensaje.msg, sizeof(MsgCanalEntradaAgente));
            if (socketAgente.send(bufferSocket, TAM_BUFFER) != TAM_BUFFER) {
                Logger::logMessage(Logger::ERROR, "Error al enviar "
                "mensajes al Agente");
                socketAgente.destroy();
                abort();
            }
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
    
    
    socketAgente.destroy();
    Logger::logMessage(Logger::COMM, "Se destruye el canal.");
    return 0;
}
