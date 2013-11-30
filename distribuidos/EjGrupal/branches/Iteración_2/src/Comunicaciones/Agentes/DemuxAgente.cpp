#include <middlewareCommon.h>
#include <Logger/Logger.h>
#include <IPCTemplate/MsgQueue.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <Common.h>

#define MAX_SIZE_DIR            20
#define MAX_SIZE_AGENT_NAME     20

class ArgumentParser {
public:
    ArgumentParser(int argc, char** argv);
    int parseArgument(int index, int & argument);
    int parseArgument(int index, long & argument);
    int parseArgument(int index, char argument[], int argumentSize);
    
private:
    bool isIndexValid(int index) {
        if (index > argc_ || index < 1) {
            return false;
        }
        return true;
    }
private:
    int argc_;
    char** argv_;
};

ArgumentParser::ArgumentParser(int argc, char** argv) 
        : argc_(argc),
          argv_(argv) {}


int ArgumentParser::parseArgument(int index, int & argument) {
    if (isIndexValid(index)) {
        sscanf(argv_[index], "%d", &argument);
    }
    else {
        return -1;
    }
    return 0; //TODO corregir!!!!!
}

int ArgumentParser::parseArgument(int index, long & argument) {
    if (isIndexValid(index)) {
        sscanf(argv_[index], "%ld", &argument);
    }
    else {
        return -1;
    }
    return 0; //TODO corregir!!!!!
}

int ArgumentParser::parseArgument(int index, char argument[], 
                                   int argumentSize) {
    if (isIndexValid(index)) {
        if ( strlen(argv_[index]) > static_cast<size_t>(argumentSize) ) {
            return -1;
        }
        else {
            strcpy(argument, argv_[index]);
        }
    }
    else {
        return -1;
    }
    return 0; //TODO corregir!!!!!
}

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
    sprintf(buffer, "DemuxAgente - TipoAgente: %d", idTipoAgente);
    Logger::setProcessInformation(buffer);
    
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
        
        msg_demux_t mensaje;
        mensaje.tamanioMensajeDemux = tamanioMensaje;
        strcpy(mensaje.mensajeDemux, buffer);
        
        msg_a_canal_t mensajeACanal;
        mensajeACanal.mtype = idEmisor;
        mensajeACanal.idTipoAgente = idTipoAgente;
        strcpy(mensajeACanal.directorioIPC, directorioIPC);
        mensajeACanal.idIPC = idIPC;
        mensajeACanal.mensaje = mensaje;
        
        // Se envía al CanalDeSalida el mensaje formado
        colaCanalDeSalida.send(mensajeACanal);
    }
    
    return 0;
}
