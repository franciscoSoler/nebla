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
    char buffer[TAM_BUFFER];
    char bufferSocket[TAM_BUFFER];
    ArgumentParser argParser(argc, argv);
    long idAgente = 0;
    int idTipoAgente = 0;
    int idBroker = 0;

    if ( argParser.parseArgument(1, idAgente) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 1");
        exit(-1);
    }
    
    if ( argParser.parseArgument(2, idTipoAgente) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 2");
        exit(-1);
    }

    if ( argParser.parseArgument(3, idBroker) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 3");
        exit(-1);
    }
  
    
    ServersManager serversManager;
    SocketStream::SocketStreamPtr socketBroker(
    serversManager.connectToBrokerServer("ServidorCanalSalidaBrokerAgente", idBroker) );
    assert( socketBroker.get() );
    
    // Se envían los datos del agente por el socket al CanalSalidaBroker
    std::stringstream ss;
    ss << idAgente << " ";
    ss << idTipoAgente;
    memcpy(bufferSocket, ss.str().c_str(), sizeof(int) + sizeof(long));
    
    sprintf(buffer, "Se envían los datos del agente: %ld - %d", 
            idAgente, idTipoAgente);
    Logger::logMessage(Logger::COMM, buffer);
    
    
    if ( socketBroker->send(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al enviar el id del agente");
        socketBroker->destroy();
        abort();
    }
    
    try {
        IPC::MsgQueue colaAgente;
    
        while( true ) {
            if ( socketBroker->receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensajes del broker");
                socketBroker->destroy();
                abort();
            }
            MsgCanalEntradaAgente mensaje;
            memcpy(&mensaje, bufferSocket, sizeof(MsgCanalEntradaAgente));

            /*msg_pedido_t pedido;
            memcpy(&pedido, mensaje.msg.msg, sizeof(msg_pedido_t));

            sprintf(buffer, "MsgPedidoT: %d - %d", pedido.tipo, pedido.pedido.tipoProducto);
            Logger::logMessage(Logger::IMPORTANT, buffer);*/
            
            sprintf(buffer, "directorioIPC: %s, idIPC: %d, mtype del mensaje a enviar %ld",
                    mensaje.directorioIPC, mensaje.idIPC, mensaje.msg.mtype);
            Logger::logMessage(Logger::COMM, buffer);
            
            colaAgente = obtenerColaAgente(mensaje.directorioIPC, mensaje.idIPC);
            
            //char buffer[TAM_BUFFER];
            //sprintf(buffer, "Recibe mensaje de Broker: mtype siguiente: %ld, ", mensaje.msg.mtype, )
            //Logger::logMessage(Logger::COMM, buffer);
            
            colaAgente.send(mensaje.msg);
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
