#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include <Common.h>
#include <middlewareCommon.h>
#include <Exceptions/Exception.h>
#include "Logger/Logger.h"
#include <API/Objects/Util.h>
#include <ConfigFileParser/ConfigFileParser.h>

#include "IPCs/IPCTemplate/MsgQueue.h"
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/ArgumentParser.h>

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_ADM = NULL;
static const char* C_DIRECTORY_INFO_AGENTES = NULL;

void elegirDirectorios(int brokerNumber);

int main(int argc, char* argv[]) {
    try {
        ArgumentParser argParser(argc, argv);
        int brokerNumber = 0;

        // Se recibe por parámetro que Broker se está inicializando
        if (argc != 2) {
            Logger::logMessage(Logger::ERROR, "Cantidad de parámetros inválidos");
            exit( -1 );
        }

        if (argParser.parseArgument(1, brokerNumber) == -1) {
            Logger::logMessage(Logger::ERROR, "Argumento inválido");
            exit( -1 );
        }

        elegirDirectorios( brokerNumber );

        Logger::getInstance().setProcessInformation("TerminatorBrokers:");
        std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser( COMM_OBJECTS_CONFIG_FILE ));
        cfg->parse();

        // Se crea una cola por cada Agente
        IPC::MsgQueue colaAgente;
        
        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_CLIENTE);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola cliente destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_VENDEDOR);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola vendedor destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_AP);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola AlmacenDePiezas destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_AGV);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola AGV destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_ROBOT5_AGV);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot5AGV destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_ROBOT5_CINTA);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot5Cinta destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_ROBOT11);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot11 destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_ROBOT12);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot12 destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_ROBOT14);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot14 destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_ROBOT16_CINTA);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot16Cinta destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_ROBOT16_DESPACHO);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Robot16Despacho destruida");

        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_DESPACHO);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Despacho destruida"); 
        
        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_MEMORIA);
        colaAgente.destroy();  
        Logger::logMessage(Logger::COMM, "Cola Memorias destruida");
   
        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_PEDIDO_MEMORIA);
        colaAgente.destroy();     
        Logger::logMessage(Logger::COMM, "Cola Pedidos Memorias destruida"); 

        std::list<int> shMemListId = cfg->getParamIntList( "shMem" );
        while ( not shMemListId.empty() ) {
            // Obtengo la memoria compartida contadora
            IPC::SharedMemory<int> contadoraSharedMemory("Contadora Pedidos ShMem");
            contadoraSharedMemory.getSharedMemory(C_DIRECTORY_ADM, shMemListId.front());
            contadoraSharedMemory.destroy();
            Logger::logMessage(Logger::COMM, "shMemContadoraPedidos destruida");

            IPC::Semaphore semaforoContadora("Semaforo Contadora Pedidos");
            semaforoContadora.getSemaphore(C_DIRECTORY_ADM, shMemListId.front(), 1);
            semaforoContadora.destroy();
            Logger::logMessage(Logger::COMM, "Semaforo Contadora Pedidos destruida");

            shMemListId.pop_front();
        }

        // Obtengo la memoria compartida con el siguiente broker
        IPC::SharedMemory<int> siguienteSharedMemory("Siguiente Broker ShMem");
        siguienteSharedMemory.getSharedMemory(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
        siguienteSharedMemory.destroy();
        Logger::logMessage(Logger::COMM, "shMem SiguienteBroker destruida");

        IPC::Semaphore semaforoSiguiente = IPC::Semaphore("Semaforo Siguiente Broker");
        semaforoSiguiente.getSemaphore(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE, 1);
        semaforoSiguiente.destroy();
        Logger::logMessage(Logger::COMM, "shMem SiguienteBroker destruida");

        // Cola para que los procesos del Broker se comuniquen con el canal de salida
        // hacia otro Broker

        IPC::MsgQueue colaCanalSalidaBrokerBroker;
        colaCanalSalidaBrokerBroker.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);
        colaCanalSalidaBrokerBroker.destroy();
        Logger::logMessage(Logger::COMM, "cola CanalSalidaBrokerBroker destruida");

        // Creación de las memorias compartidas que poseen información sobre agentes
        // conectados
        IPC::Semaphore semMutexShMemInfoAgentes;
        semMutexShMemInfoAgentes.getSemaphore(C_DIRECTORY_INFO_AGENTES, ID_INFO_AGENTES, AMOUNT_AGENTS);
        semMutexShMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "sem InfoAgentes destruído");

        IPC::SharedMemory<DataInfoAgentes> shMemInfoAgentes;

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_CLIENTE);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Cliente destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_VENDEDOR);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Vendedor destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_AP);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-AP destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_AGV);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-AGV destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT5_CINTA);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot5_Cinta destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT5_AGV);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot5_AGV destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT11);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot11 destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT12);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot12 destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT14);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot14 destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT16_CINTA);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot16_Cinta destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT16_DESPACHO);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot16_Despacho destruída");

        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_DESPACHO);
        shMemInfoAgentes.destroy();
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Despacho destruída");

        /* Matriz para identificar qué brokers pertenecen a cada grupo de ShMem. */
        IPC::SharedMemory<InformacionGrupoShMemBrokers> shMemInfoGruposShMemBrokers;
        shMemInfoGruposShMemBrokers.getSharedMemory(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS);
        shMemInfoGruposShMemBrokers.destroy();
        Logger::logMessage(Logger::COMM, "shMem InforGruposBrokers creada.");

        IPC::Semaphore semInfoGruposShMemBrokers;
        semInfoGruposShMemBrokers.getSemaphore(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS, 1);
        semInfoGruposShMemBrokers.destroy();
        Logger::logMessage(Logger::COMM, "sem InforGruposBrokers creado.");
        
        ServersManager serversManager;
        serversManager.killServers( brokerNumber );

    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    } 
    return 0;
}

void elegirDirectorios(int brokerNumber) {
    switch (brokerNumber) {
        case 1:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_1;
            C_DIRECTORY_ADM = DIRECTORY_ADM_1;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_1;
            break;
        case 2:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_2;
            C_DIRECTORY_ADM = DIRECTORY_ADM_2;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_2;
            break;
        case 3:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_3;
            C_DIRECTORY_ADM = DIRECTORY_ADM_3;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_3;
            break;
        case 4:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_4;
            C_DIRECTORY_ADM = DIRECTORY_ADM_4;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_4;
            break;
        default:
            Logger::logMessage(Logger::ERROR, "Error al elegir directorios del Broker");
            abort();
    }
}
    
