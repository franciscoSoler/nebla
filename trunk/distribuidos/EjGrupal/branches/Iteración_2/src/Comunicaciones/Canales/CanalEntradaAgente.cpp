#include <iostream>
#include <string.h>
#include <assert.h>
#include <sstream>

#include <Logger/Logger.h>
#include <Common.h>

#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Socket/SocketStream.h>

#include "middlewareCommon.h"

IPC::MsgQueue obtenerColaAgente(char dirIPC[], int idIPC);

int main(int argc, char* argv[]) {
    Logger::setProcessInformation("CanalEntradaAgente:");
    char buffer[255];
    ArgumentParser argParser(argc, argv);
    long idAgente = 0;
    int idTipoAgente;

    if ( argParser.parseArgument(1, idAgente) == -1 ) {
        exit(-1);
    }
    
    if ( argParser.parseArgument(2, idTipoAgente) == -1 ) {
        exit(-1);
    }
  
    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketBroker(
    serversManager.connectToServer("ServidorCanalSalidaBroker") );
    assert( socketBroker.get() );
    
    // Se envían los datos del agente por el socket al CanalSalidaBroker
    std::stringstream ss;
    ss << idAgente;
    ss << idTipoAgente;
    memcpy(buffer, ss.str().c_str(), sizeof(int) + sizeof(long));
    
    sprintf(buffer, "Se envían los datos del agente: %ld - %d", 
            idAgente, idTipoAgente);
    Logger::logMessage(Logger::COMM, buffer);
    
    
    if ( socketBroker->send(buffer, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al enviar el id del agente");
        socketBroker->destroy();
        abort();
    }
    
    try {
        IPC::MsgQueue colaAgente;
    
        while( true ) {
            if ( socketBroker->receive(buffer, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al "
                        "recibir mensajes del broker");
                socketBroker->destroy();
                abort();
            }
            Logger::logMessage(Logger::COMM, "Recibe mensaje de Broker");
            
            MsgCanalEntradaAgente mensaje;
            colaAgente = obtenerColaAgente(mensaje.directorioIPC, mensaje.idIPC);
            colaAgente.send(mensaje.msg.mensajeMux, mensaje.msg.tamanioMensajeMux);
        }
        
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }   

    return 0;
}

IPC::MsgQueue obtenerColaAgente(char dirIPC[], int idIPC) {
    // FIXME: Crear estructura que permita reutilizar colas ya creadas
    IPC::MsgQueue colaAgente("colaAgente");
    colaAgente.getMsgQueue(dirIPC, idIPC);
    return colaAgente;
}