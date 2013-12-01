#include <iostream>
#include <string.h>
#include <assert.h>

#include <Logger/Logger.h>
#include <Common.h>

#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Objects/ArgumentParser.h>
#include <middlewareCommon.h>
#include <Socket/SocketStream.h>


int main(int argc, char* argv[]) {
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
    serversManager.connectToServer("ServidorCanalEntradaBroker") );
    assert( socketBroker.get() );
    
    sprintf(buffer, "CanalSalidaAgente - TipoAgente %d - idAgente %ld:",
            idTipoAgente, idAgente);
    Logger::setProcessInformation(buffer);
    Logger::logMessage(Logger::COMM, "Canal creado");
    
    try {
        IPC::MsgQueue colaCanalDeSalida("colaCanalSalida");
        colaCanalDeSalida.getMsgQueue(DIRECTORY_COMM, idTipoAgente); 
        
        while ( true ) {
            MsgCanalSalidaAgente mensaje;
            colaCanalDeSalida.recv(idAgente, mensaje);
            Logger::logMessage(Logger::COMM, "Recibio un mensaje");
            
            memcpy(buffer, & mensaje.msg, sizeof(MsgCanalEntradaBroker));
            socketBroker->send(buffer, sizeof(MsgCanalEntradaBroker));
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }
        

    return 0;
}
    
