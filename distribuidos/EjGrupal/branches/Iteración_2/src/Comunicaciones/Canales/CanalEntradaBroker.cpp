#include <iostream>
#include <string.h>
#include <assert.h>

#include <Logger/Logger.h>
#include <Common.h>

#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Socket/SocketStream.h>

#include "middlewareCommon.h"

IPC::MsgQueue obtenerColaAgente(char dirIPC[], int idIPC);

int main(int argc, char* argv[]) {
    Logger::setProcessInformation("CanalEntradaBroker:");
    char buffer[255];
    CommunicationsUtil util;
    int idSd = 0;

    if ( util.parseArgs(argc, argv, idSd) == -1 ) {
        exit(-1);
    }

    SocketStream socketAgente(idSd);
    Logger::logMessage(Logger::COMM, "Canal de Entrada conectado");
    
    try {
        IPC::MsgQueue colaAgente;
        while ( true ) {
            if (socketAgente.receive(buffer, TAM_BUFFER) != TAM_BUFFER) {
                Logger::logMessage(Logger::ERROR, "Error al recibir "
                "mensajes del Agente");
                socketAgente.destroy();
                abort();
            }
            Logger::logMessage(Logger::COMM, "Recibe mensaje de Agente");
            
            MsgCanalEntradaBroker mensaje;
            memcpy(&mensaje, buffer, sizeof(MsgCanalEntradaBroker));
            
            colaAgente.getMsgQueue(DIRECTORY_BROKER, mensaje.idTipoAgente);
            colaAgente.send(mensaje.msg);
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
    