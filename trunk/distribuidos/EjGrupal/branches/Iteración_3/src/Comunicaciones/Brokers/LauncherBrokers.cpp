#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory>
#include <sstream>

#include <Common.h>
#include <middlewareCommon.h>
#include <Exceptions/Exception.h>
#include <Logger/Logger.h>
#include <ConfigFileParser/ConfigFileParser.h>

#include <API/Objects/Util.h>
// Para el hardcord
#include <API/Objects/DataSM_R11_R14.h>
#include <API/Objects/DataSM_R14_R16.h>

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/ArgumentParser.h>

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_ADM = NULL;
static const char* C_DIRECTORY_INFO_AGENTES = NULL;

void createIPCs();
void createDirectory(std::string directoryName);
void createSharedMemoryAdministrators(int brokerNumber);
// Hardcodeo de la inicializacion de las memorias compartidas (Esto lo deberia hacer el lider)
void initializeSharedMemories();
void initializeBroker(int brokerNumber);
void elegirDirectorios(int brokerNumber);

int main(int argc, char* argv[]) {
    try {
        Util::getInstance();
        ArgumentParser argParser(argc, argv);
        int brokerNumber;

        // Se recibe por parámetro que Broker se está inicializando
        if (argc != 2) {
            Logger::logMessage(Logger::ERROR, "Cantidad de parámetros inválidos");
            exit( -1 );
        }

        if (argParser.parseArgument(1, brokerNumber) == -1) {
            Logger::logMessage(Logger::ERROR, "Argumento inválido");
            exit( -1 );
        }

        char buffer[255];
        sprintf(buffer, "LauncherBroker Nº%d:", brokerNumber);
        Logger::setProcessInformation(buffer);

        elegirDirectorios( brokerNumber );        
        createDirectory(C_DIRECTORY_ADM);
        createDirectory(C_DIRECTORY_BROKER);
        createDirectory(C_DIRECTORY_INFO_AGENTES);
        createIPCs();

        // Se crean los servidores para recibir conexiones de otros Brokers, y
        // se intenta conectar con los mismos.
        initializeBroker( brokerNumber );
        
        // Obtengo la memoria compartida con el siguiente broker
        IPC::SharedMemory<int> siguienteSharedMemory("Siguiente Broker ShMem");
        siguienteSharedMemory.getSharedMemory(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
        Logger::logMessage(Logger::COMM, "shMem SiguienteBroker creado");

        int siguiente = (brokerNumber % 4)+1;
        siguienteSharedMemory.write(&siguiente);
        
        createSharedMemoryAdministrators( brokerNumber );

        
        if ( brokerNumber == 1 ) {
            // Se hardcodea momentaneamente la inicializacion de las shMem
            // por falta del algoritmo del lider. Ahora que tenemos muchos Brokers
            // obligo a que el broker N°1 sea el "LIDER".
            initializeSharedMemories();
        }

        ServersManager serversManager;
        serversManager.createBrokerServer("ServidorCanalEntradaBrokerAgente", brokerNumber);
        serversManager.createBrokerServer("ServidorCanalSalidaBrokerAgente", brokerNumber);
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
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser( COMM_OBJECTS_CONFIG_FILE ));
    cfg->parse();
    
    try {
        // Se crea una cola por cada Agente
        IPC::MsgQueue colaAgente;

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_CLIENTE);
        Logger::logMessage(Logger::COMM, "Cola cliente creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_VENDEDOR);
        Logger::logMessage(Logger::COMM, "Cola vendedor creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_AP);
        Logger::logMessage(Logger::COMM, "Cola AlmacenDePiezas creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_AGV);
        Logger::logMessage(Logger::COMM, "Cola AGV creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_ROBOT5_AGV);
        Logger::logMessage(Logger::COMM, "Cola Robot5AGV creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_ROBOT5_CINTA);
        Logger::logMessage(Logger::COMM, "Cola Robot5Cinta creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_ROBOT11);
        Logger::logMessage(Logger::COMM, "Cola Robot11 creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_ROBOT12);
        Logger::logMessage(Logger::COMM, "Cola Robot12 creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_ROBOT14);
        Logger::logMessage(Logger::COMM, "Cola Robot14 creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_ROBOT16_CINTA);
        Logger::logMessage(Logger::COMM, "Cola Robot16Cinta creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_ROBOT16_DESPACHO);
        Logger::logMessage(Logger::COMM, "Cola Robot16Despacho creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_DESPACHO);
        Logger::logMessage(Logger::COMM, "Cola Despacho creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_MEMORIA);
        Logger::logMessage(Logger::COMM, "Cola Memorias creada");

        colaAgente.create(C_DIRECTORY_BROKER, ID_TIPO_PEDIDO_MEMORIA);
        Logger::logMessage(Logger::COMM, "Cola Pedidos Memorias creada");

        std::list<int> shMemIdList = cfg->getParamIntList("shMem");
        while ( not shMemIdList.empty() ) {
            IPC::SharedMemory<int> contadoraSharedMemory("Contadora Pedidos ShMem");
            contadoraSharedMemory.createSharedMemory(C_DIRECTORY_ADM, shMemIdList.front());
            Logger::logMessage(Logger::COMM, "shMemContadoraPedidos creada");

            IPC::Semaphore semaforoContadora("Semaforo Contadora Pedidos");
            semaforoContadora.createSemaphore(C_DIRECTORY_ADM, shMemIdList.front(), 1);
            semaforoContadora.initializeSemaphore(0, 1);
            Logger::logMessage(Logger::COMM, "Semaforo Contadora Pedidos creado");

            shMemIdList.pop_front();
        }

        // Obtengo la memoria compartida con el siguiente broker
        IPC::SharedMemory<int> siguienteSharedMemory("Siguiente Broker ShMem");
        siguienteSharedMemory.createSharedMemory(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
        Logger::logMessage(Logger::COMM, "shMem SiguienteBroker creado");

        IPC::Semaphore semaforoSiguiente = IPC::Semaphore("Semaforo Siguiente Broker");
        semaforoSiguiente.createSemaphore(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE, 1);
        semaforoSiguiente.initializeSemaphore(0, 1);
        Logger::logMessage(Logger::COMM, "Semaforo shMem SiguienteBroker creado");
               

        // Cola para que los procesos del Broker se comuniquen con el canal de salida
        // hacia otro Broker
        IPC::MsgQueue colaCanalSalidaBrokerBroker("colaCanalSalidaBrokerBroker");
        colaCanalSalidaBrokerBroker.create(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);

        // Creación de las memorias compartidas que poseen información sobre agentes
        // conectados
        IPC::Semaphore semMutexShMemInfoAgentes("semMutexShMemInfoAgentes");
        semMutexShMemInfoAgentes.createSemaphore(C_DIRECTORY_INFO_AGENTES, ID_INFO_AGENTES, AMOUNT_AGENTS);
        for (int i = 0; i < AMOUNT_AGENTS; ++i) {
            semMutexShMemInfoAgentes.initializeSemaphore(i, 1);
        }
        Logger::logMessage(Logger::COMM, "sem InfoAgentes creado");


        IPC::SharedMemory<DataInfoAgentes> shMemInfoAgentes;

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_CLIENTE);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Cliente creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_VENDEDOR);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Vendedor creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_AP);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-AP creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_AGV);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-AGV creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT5_CINTA);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot5_Cinta creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT5_AGV);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot5_AGV creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT11);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot11 creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT12);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot12 creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT14);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot14 creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT16_CINTA);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot16_Cinta creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_ROBOT16_DESPACHO);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Robot16_Despacho creada");

        shMemInfoAgentes.createSharedMemory(C_DIRECTORY_INFO_AGENTES, ID_TIPO_DESPACHO);
        Logger::logMessage(Logger::COMM, "shMem InfoAgentes-Despacho creada");

        /* Matriz para identificar qué brokers pertenecen a cada grupo de ShMem. */
        IPC::SharedMemory<InformacionGrupoShMemBrokers> shMemInfoGruposShMemBrokers;
        shMemInfoGruposShMemBrokers.createSharedMemory(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS);
        Logger::logMessage(Logger::COMM, "shMem InforGruposBrokers creada.");

        IPC::Semaphore semInfoGruposShMemBrokers;
        semInfoGruposShMemBrokers.createSemaphore(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS, 1);
        Logger::logMessage(Logger::COMM, "sem InforGruposBrokers creado.");

    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, e.get_error_description().c_str());
        abort();
    }
}

void createSharedMemoryAdministrators(int brokerNumber) {
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE) );
    cfg->parse();
    std::list<int> sharedMemoryListIds = cfg->getParamIntList("shMem");
    int listSize = sharedMemoryListIds.size();

    for (int i = 0; i < listSize; ++i) {
        std::stringstream ssSharedMemoryId;
        ssSharedMemoryId << sharedMemoryListIds.front();

        std::stringstream ssBrokerNumber;
        ssBrokerNumber << brokerNumber;

        Util::createProcess("AdministradorMemoria",
                            ssSharedMemoryId.str().c_str(),
                            ssBrokerNumber.str().c_str());
        sharedMemoryListIds.pop_front();
    }
}

void initializeSharedMemories() {
    char buffer[MSG_BROKER_SIZE];
    MsgEntregaMemoriaAdministrador mensajeMemoria;
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE) );
    cfg->parse();

    IPC::MsgQueue colaMemoria = IPC::MsgQueue("Cola Memoria");
    colaMemoria.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_MEMORIA);

    // Estado Robot5
    EstadoRobot5 estadoRobot5;
    estadoRobot5.robot5Bloqueado = false;
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DRobot5-4", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &estadoRobot5, sizeof(EstadoRobot5));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // Cinta 6-0
    CintaTransportadora_6 cinta6;
    cinta6.cantLibres = BUFF_SIZE_CINTA_6;
    cinta6.puntoLectura = 0;
    cinta6.robot11Durmiendo = false;
    for (int i = 0; i < BUFF_SIZE_CINTA_6; i++) cinta6.lugarVacio[i] = true;
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DRobot11-2", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &cinta6, sizeof(CintaTransportadora_6));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // Cinta 6-1
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DRobot11-3", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &cinta6, sizeof(CintaTransportadora_6));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // BufferCanastoEntre5yAGVSharedMemory 0 (No veo donde se inicializa...)
    Canasto canasto;
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DAGV-3", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &canasto, sizeof(Canasto));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // BufferCanastoEntre5yAGVSharedMemory 1 (No veo donde se inicializa...)
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DAGV-4", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &canasto, sizeof(Canasto));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // BufferCanastoEntre5yAGVSharedMemory 2 (No veo donde se inicializa...)
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DAGV-5", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &canasto, sizeof(Canasto));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);


    // BufferCanastos 0
    BufferCanastos canastos;
    for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++) {
        canastos.canastos[i].cantidadPiezas = 0;
        canastos.canastos[i].tipoPieza = NULL_PIEZA;
        canastos.robotCinta1EsperaPorElLugarNumero = -1;
        canastos.robotCinta2EsperaPorElLugarNumero = -1;
    }
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DAGV-9", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &canastos, sizeof(BufferCanastos));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // BufferCanastos 1
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DAGV-10", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &canastos, sizeof(BufferCanastos));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // BufferCanastos 2
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DAGV-11", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &canastos, sizeof(BufferCanastos));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // SM-R11-R14
    DataSM_R11_R14 dataSM_R11_R14;
    SerializedData data = dataSM_R11_R14.serializeData();
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DRobot11-6", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &data, sizeof(SerializedData));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    
    // SM-R14-R16
    DataSM_R14_R16 dataSM_R14_R16;
    data = dataSM_R14_R16.serializeData();
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DRobot14-2", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &data, sizeof(SerializedData));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // AlmacenTerminados (No se lo inicializa en el Launcher, lo mismo se repite aqui)
    AlmacenProductosTerminados apt;
    
    for (int i = 0; i < TAM_ALMACEN; i++) {
        apt.almacen[i].esTemporal = false;
        apt.almacen[i].estado = VACIO;
        apt.almacen[i].idOrdenDeCompra = 0;
        apt.almacen[i].tipoProducto = 0;
    }
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DVendedor-1", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &apt, sizeof(AlmacenProductosTerminados));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // NumeroOrdenCompra
    int shMemData = 1;
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DVendedor-2", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &shMemData, sizeof(int));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // AlmacenDePiezas
    EstructuraAlmacenPiezas estructuraAlmacen;
    for (int i = 0; i < CANTIDAD_TIPOS_PIEZAS; ++i) estructuraAlmacen.cantCanastos[i] = 1;
    for (int i = 0; i < CANTIDAD_TIPOS_GABINETES; ++i) estructuraAlmacen.cantGabinetes[i] = 0;
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DAPiezas-1", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &estructuraAlmacen, sizeof(EstructuraAlmacenPiezas));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

}

void initializeBroker(int brokerNumber) {
    ServersManager serversManager;
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser( SERVERS_CONFIG_FILE ));
    cfg->parse();

    int amountBrokers = cfg->getConfigFileParam("CantidadBrokers", -1);
    if ( amountBrokers != -1 ) {
        std::string inputServer = "ServidorCanalEntradaBrokerBroker";
        std::string outputServer = "ServidorCanalSalidaBrokerBroker";

        // Create the servers to receive the connections from other Brokers
        serversManager.createBrokerServer(inputServer.c_str(), brokerNumber);
        serversManager.createBrokerServer(outputServer.c_str(), brokerNumber);

        // Then, connect to other Brokers
        for (int j = 1; j <= amountBrokers; ++j) {
            if (brokerNumber != j) {
                serversManager.createChannelToBroker("CanalEntradaBrokerBroker",
                                                     brokerNumber, j);
                serversManager.createChannelToBroker("CanalSalidaBrokerBroker",
                                                     brokerNumber, j);
            }
        }
    }
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


