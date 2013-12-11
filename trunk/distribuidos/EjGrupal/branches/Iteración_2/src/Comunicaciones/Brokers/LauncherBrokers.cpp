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
// Para el hardcord
#include <API/Objects/DataSM_R11_R14.h>
#include <API/Objects/DataSM_R14_R16.h>

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>

#include <Comunicaciones/Objects/ServersManager.h>

void createIPCs();
void createDirectory(std::string directoryName);
void createSharedMemoryAdministrators();
// Hardcodeo de la inicializacion de las memorias compartidas (Esto lo deberia hacer el lider)
void initializeSharedMemories();

int main(int argc, char* argv[]) {
    try {
        Util::getInstance();
        
        createDirectory(DIRECTORY_ADM);
        createDirectory(DIRECTORY_BROKER);
        
        createIPCs();
        createSharedMemoryAdministrators();
        // Se hardcodea momentaneamente la inicializacion de las shMem
        // por falta del algoritmo del lider
        initializeSharedMemories();

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
            semaforoContadora.initializeSemaphore(0, 1);
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

void initializeSharedMemories() {
    char buffer[MSG_BROKER_SIZE];
    MsgEntregaMemoriaAdministrador mensajeMemoria;
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE) );
    cfg->parse();

    IPC::MsgQueue colaMemoria = IPC::MsgQueue("Cola Memoria");
    colaMemoria.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_MEMORIA);

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
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DRobot11-6", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &dataSM_R11_R14, sizeof(DataSM_R11_R14));
    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);

    // SM-R14-R16
    DataSM_R14_R16 dataSM_R14_R16;
    mensajeMemoria.mtype = cfg->getConfigFileParam("shMem-./DRobot14-2", -1);
    sprintf(buffer, "Se manda mensaje de inicio "
            "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    memcpy(mensajeMemoria.memoria, &dataSM_R14_R16, sizeof(DataSM_R14_R16));
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
