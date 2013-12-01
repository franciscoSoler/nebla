#include <middlewareCommon.h>
#include <Logger/Logger.h>
#include <IPCTemplate/MsgQueue.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <Common.h>
#include <Exceptions/Exception.h>

#include <Comunicaciones/Objects/ArgumentParser.h>

#define MAX_SIZE_DIR            20
#define MAX_SIZE_AGENT_NAME     20

MsgAgenteReceptor crearMsgAgenteReceptor(char buffer[], int tamanioMensaje, 
                                           long idReceptor);

int main(int argc, char* argv[]) {
    char directorioIPC[MAX_SIZE_DIR];
    int idIPC;
    int tamanioMensaje;
    //TipoAgente idTipoAgente;
    int idTipoAgent;
    
    ArgumentParser argParser(argc, argv);
    int parserResults = 0;
    
    parserResults += argParser.parseArgument(1, directorioIPC, MAX_SIZE_DIR);
    parserResults += argParser.parseArgument(2, idIPC);
    parserResults += argParser.parseArgument(3, tamanioMensaje);
    parserResults += argParser.parseArgument(4, idTipoAgent);
    TipoAgente idTipoAgente = static_cast<TipoAgente> (idTipoAgent);

    if ( parserResults < 0) {
        Logger::logMessage(Logger::ERROR, "Parseo incorrecto de parámetros");
        exit(-1);
    }
    
    char buffer[255];
    sprintf(buffer, "MuxTipoAgente - TipoAgente: %d", idTipoAgente);
    Logger::setProcessInformation(buffer);
    
    try {
        // Creo la colas con los parámetros parseados
        IPC::MsgQueue colaAgente("colaAgente");
        colaAgente.getMsgQueue(directorioIPC, idIPC);

        IPC::MsgQueue colaCanalDeSalida("colaCanalSalida");
        colaCanalDeSalida.getMsgQueue(DIRECTORY_COMM, idTipoAgente);

        while ( true ) {
            char buffer[TAM_BUFFER];
            colaAgente.recv(MSG_DEMUX, buffer, tamanioMensaje);
            Logger::logMessage(Logger::COMM, "Recibe mensaje");

            // El proceso procede a rearmar el mensaje para enviarselo al 
            // CanalDeSalida
            std::stringstream ss;
            ss << buffer;

            long idReceptor;
            long idEmisor;

            ss >> idReceptor;
            ss >> idReceptor;
            ss >> idEmisor;

            if ( idReceptor < 0 || idEmisor < 0) {
                Logger::logMessage(Logger::ERROR, "Se recibió un "
                "mensaje malformado de la colaAgente");
                abort();
            }
            
            MsgAgenteReceptor msgAgenteReceptor = crearMsgAgenteReceptor(
                              buffer, tamanioMensaje, idReceptor);
            
            MsgCanalEntradaAgente msgCanalEntradaAgente;
            strcpy(msgCanalEntradaAgente.directorioIPC, directorioIPC);
            msgCanalEntradaAgente.idIPC = idIPC;
            msgCanalEntradaAgente.msg = msgAgenteReceptor;
            
            MsgCanalSalidaBroker msgCanalSalidaBroker;
            msgCanalSalidaBroker.mtype = idReceptor;
            msgCanalSalidaBroker.msg = msgCanalEntradaAgente;
            
            
            MsgCanalEntradaBroker msgCanalEntradaBroker;
            msgCanalEntradaBroker.idReceptor = idReceptor;
            msgCanalEntradaBroker.idTipoAgente = idTipoAgente;
            msgCanalEntradaBroker.msg = msgCanalSalidaBroker;
            
            MsgCanalSalidaAgente msgCanalSalidaAgente;
            msgCanalSalidaAgente.mtype = idEmisor;
            msgCanalSalidaAgente.msg = msgCanalEntradaBroker;
   
            // Se envía al CanalDeSalida el mensaje formado
            colaCanalDeSalida.send(msgCanalSalidaAgente);
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
    
    return 0;
}

MsgAgenteReceptor crearMsgAgenteReceptor(char buffer[], int tamanioMensaje, 
                                            long idReceptor) {
    
    memcpy(buffer, &idReceptor, sizeof(long));
    MsgAgenteReceptor msg;
    strcpy(msg.mensajeMux, buffer);
    msg.tamanioMensajeMux = tamanioMensaje;
    return msg;
}