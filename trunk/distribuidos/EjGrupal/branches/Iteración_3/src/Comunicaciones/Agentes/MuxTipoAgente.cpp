#include <middlewareCommon.h>
#include <Logger/Logger.h>
#include <Comunicaciones/Objects/CommMsgQueue.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <Common.h>
#include <assert.h>
#include <Exceptions/Exception.h>

#include <Comunicaciones/Objects/ArgumentParser.h>

#define MAX_SIZE_DIR            20
#define MAX_SIZE_AGENT_NAME     20

int main(int argc, char* argv[]) {
    int idTipoAgent;
    
    ArgumentParser argParser(argc, argv);
    int parserResults = 0;
    
    parserResults += argParser.parseArgument(1, idTipoAgent);
    TipoAgente idTipoAgente = static_cast<TipoAgente> (idTipoAgent);

    if ( parserResults < 0) {
        Logger::logMessage(Logger::ERROR, "Parseo incorrecto de parámetros");
        exit(-1);
    }
    
    
    char buffer[255];
    sprintf(buffer, "MuxTipoAgente - TipoAgente: %d", idTipoAgent);
    Logger::setProcessInformation(buffer);
    Logger::logMessage(Logger::COMM, "Creando Mux");
    
    try {
        // Creo la colas con los parámetros parseados
        IPC::MsgQueue colaAgente("colaAgente", 0, idTipoAgente);
        colaAgente.getMsgQueue(DIRECTORY_MUX, idTipoAgente);

        IPC::CommMsgQueue colaCanalDeSalida("colaCanalSalida");
        colaCanalDeSalida.getMsgQueue(DIRECTORY_COMM, idTipoAgente);

        while ( true ) {
            // char buffer[TAM_BUFFER];
            MsgAgenteReceptor msgAgenteReceptor;
            colaAgente.recv(MSG_MUX, msgAgenteReceptor);
            msgAgenteReceptor.mtype = msgAgenteReceptor.idReceptor;
            Logger::logMessage(Logger::COMM, "Recibe mensaje");
            
            //memcpy(msgAgenteReceptor.msg, &(msgAgenteReceptor.idReceptor), sizeof(long));
            long idReceptor = msgAgenteReceptor.idReceptor;
            long idEmisor = msgAgenteReceptor.idEmisor;
            
            MsgCanalEntradaAgente msgCanalEntradaAgente;
            strcpy(msgCanalEntradaAgente.directorioIPC, 
                   msgAgenteReceptor.dirIPCReceptor);
            msgCanalEntradaAgente.idIPC = msgAgenteReceptor.idIPCReceptor;
            memcpy(&msgCanalEntradaAgente.msg, &msgAgenteReceptor, sizeof(MsgAgenteReceptor));
            
            MsgCanalSalidaBroker msgCanalSalidaBroker;
            msgCanalSalidaBroker.mtype = idReceptor;
            msgCanalSalidaBroker.msg = msgCanalEntradaAgente;
            memcpy(& msgCanalSalidaBroker.msg, &msgCanalEntradaAgente, sizeof(MsgCanalEntradaAgente));
            
            MsgCanalEntradaBroker msgCanalEntradaBroker;
            msgCanalEntradaBroker.idReceptor = idReceptor;
            msgCanalEntradaBroker.idTipoReceptor = msgAgenteReceptor.idTipoReceptor;
            memcpy(&msgCanalEntradaBroker.msg, &msgCanalSalidaBroker, sizeof(MsgCanalSalidaBroker));
            
            MsgCanalSalidaAgente msgCanalSalidaAgente;
            msgCanalSalidaAgente.mtype = idEmisor;
            memcpy(&msgCanalSalidaAgente.msg, &msgCanalEntradaBroker, sizeof(MsgCanalEntradaBroker));

            sprintf(buffer, "parametros mensaje Salida - idRec: %ld, idEmis: %ld, "
                    "idIPCRec: %d, dirIPCReceptor: %s",
                    msgCanalSalidaAgente.msg.msg.msg.msg.idReceptor,
                    msgCanalSalidaAgente.msg.msg.msg.msg.idEmisor,
                    msgCanalSalidaAgente.msg.msg.msg.msg.idIPCReceptor,
                    msgCanalSalidaAgente.msg.msg.msg.msg.dirIPCReceptor);
            Logger::logMessage(Logger::COMM, buffer);


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
