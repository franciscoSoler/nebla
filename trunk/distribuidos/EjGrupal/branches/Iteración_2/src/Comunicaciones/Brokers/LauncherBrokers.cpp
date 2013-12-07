#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory>

#include <Common.h>
#include <middlewareCommon.h>
#include <Exceptions/Exception.h>
#include "Logger/Logger.h"
#include <ConfigFileParser/ConfigFileParser.h>

#include <API/Objects/Util.h>
#include "IPCs/IPCTemplate/MsgQueue.h"

#include <Comunicaciones/Objects/ServersManager.h>

void createIPCs();
void createDirectory(std::string directoryName);
void createSharedMemoryAdministrators();

int main(int argc, char* argv[]) {
    try {
        Util::getInstance();
        
        createDirectory(DIRECTORY_ADM);
        createDirectory(DIRECTORY_BROKER);
        
        createIPCs();
        //createSharedMemoryAdministrators();

        ServersManager serversManager;
        serversManager.createServer("ServidorCanalEntradaBroker");
        serversManager.createServer("ServidorCanalSalidaBroker");
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, 
        e.get_error_description().c_str());
        return -1;
    }

    return 0;
}

void createDirectory(std::string directoryName) {
    mkdir(directoryName.c_str(), 0777);
}

void createIPCs() {
    Logger::getInstance().setProcessInformation("LauncherBrokers:");
    
    // Se crea una cola por cada Agente
    IPC::MsgQueue colaAgente;
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_CLIENTE);
    Logger::logMessage(Logger::COMM, "Cola cliente creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_VENDEDOR);
    Logger::logMessage(Logger::COMM, "Cola vendedor creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_AP);
    Logger::logMessage(Logger::COMM, "Cola AlmacenDePiezas creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_AGV);
    Logger::logMessage(Logger::COMM, "Cola AGV creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_ROBOT5_AGV);
    Logger::logMessage(Logger::COMM, "Cola Robot5AGV creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_ROBOT5_CINTA);
    Logger::logMessage(Logger::COMM, "Cola Robot5Cinta creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_ROBOT11);
    Logger::logMessage(Logger::COMM, "Cola Robot11 creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_ROBOT12);
    Logger::logMessage(Logger::COMM, "Cola Robot12 creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_ROBOT14);
    Logger::logMessage(Logger::COMM, "Cola Robot14 creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_ROBOT16_CINTA);
    Logger::logMessage(Logger::COMM, "Cola Robot16Cinta creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_ROBOT16_DESPACHO);
    Logger::logMessage(Logger::COMM, "Cola Robot16Despacho creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_DESPACHO);
    Logger::logMessage(Logger::COMM, "Cola Despacho creada"); 

    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_MEMORIA);
    Logger::logMessage(Logger::COMM, "Cola Memorias creada");
    
    colaAgente.create(DIRECTORY_BROKER, ID_TIPO_PEDIDO_MEMORIA);
    Logger::logMessage(Logger::COMM, "Cola Pedidos Memorias creada");    

    // TODO: Crear todas las memorias compartidas de contadoras bajo DIRECTORY_ADM
}

void createSharedMemoryAdministrators() {
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(SH_MEM_CONFIG_FILE) );
    std::list<int> sharedMemoryListIds = cfg->getParamIntList("shMem");
    int listSize = sharedMemoryListIds.size();

    for (int i = 0; i < listSize; ++i) {
        Util::createProcess("AdministradorMemoria", sharedMemoryListIds.front());
        sharedMemoryListIds.pop_front();
    }
}

