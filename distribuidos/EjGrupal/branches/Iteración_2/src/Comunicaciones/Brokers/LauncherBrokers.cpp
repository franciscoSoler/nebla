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

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>

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
        createSharedMemoryAdministrators();

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
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser( COMM_OBJECTS_CONFIG_FILE ));
    cfg->parse();
    
    try {
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

        std::list<int> shMemIdList = cfg->getParamIntList("shMem");
        while ( not shMemIdList.empty() ) {
            IPC::SharedMemory<int> contadoraSharedMemory("Contadora Pedidos ShMem");
            contadoraSharedMemory.createSharedMemory(DIRECTORY_ADM, shMemIdList.front());
            Logger::logMessage(Logger::COMM, "shMemContadoraPedidos creada");

            IPC::Semaphore semaforoContadora("Semaforo Contadora Pedidos");
            semaforoContadora.createSemaphore(DIRECTORY_ADM, shMemIdList.front(), 1);
            Logger::logMessage(Logger::COMM, "Semaforo Contadora Pedidos creado");

            shMemIdList.pop_front();
        }

        // Obtengo la memoria compartida con el siguiente broker
        IPC::SharedMemory<int> siguienteSharedMemory("Siguiente Broker ShMem");
        siguienteSharedMemory.createSharedMemory(DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
        Logger::logMessage(Logger::COMM, "shMem SiguienteBroker creado");

        IPC::Semaphore semaforoSiguiente = IPC::Semaphore("Semaforo Siguiente Broker");
        semaforoSiguiente.createSemaphore(DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE, 1);
        Logger::logMessage(Logger::COMM, "shMem SiguienteBroker creado");
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, e.get_error_description().c_str());
        abort();
    }
}

void createSharedMemoryAdministrators() {
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE) );
    cfg->parse();
    std::list<int> sharedMemoryListIds = cfg->getParamIntList("shMem");
    int listSize = sharedMemoryListIds.size();

    for (int i = 0; i < listSize; ++i) {
        Util::createProcess("AdministradorMemoria", 1, sharedMemoryListIds.front());
        sharedMemoryListIds.pop_front();
    }
}
