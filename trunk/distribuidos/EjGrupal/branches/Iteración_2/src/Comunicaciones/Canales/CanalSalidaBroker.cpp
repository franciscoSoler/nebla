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
    Logger::setProcessInformation("CanalSalidaBroker:");
    char buffer[255];
    CommunicationsUtil util;
    int idSd = 0;
    long idAgente = 0;
    int idTipoAgente;

    if ( util.parseArgs(argc, argv, idSd) == -1 ) {
        exit(-1);
    }

    SocketStream socketAgente(idSd);
    Logger::logMessage(Logger::COMM, "Canal de Salida conectado");
    
    // Recibe el id los datos del Agente
    if ( socketAgente.receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al recibir el id del Agente");
        socketAgente.destroy();
        abort();
    }
    
    std::stringstream ss;
    ss << buffer;
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
            colaBroker.recv(idAgente, mensaje);
            Logger::logMessage(Logger::COMM, "Recibe mensaje de Broker");
            
            memcpy(buffer, &mensaje.msg, sizeof(MsgCanalEntradaAgente));
            if (socketAgente.send(buffer, TAM_BUFFER) != TAM_BUFFER) {
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