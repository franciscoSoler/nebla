#include <iostream>
#include <string.h>
#include <assert.h>
#include <sstream>
#include <memory>

#include <Logger/Logger.h>
#include <ConfigFileParser/ConfigFileParser.h>
#include <Common.h>

#include <IPCs/IPCTemplate/MsgQueue.h>
#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Comunicaciones/Objects/CommPacketWrapper.h>
#include <Socket/SocketStream.h>


#include "middlewareCommon.h"

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_INFO_AGENTES = NULL;
static const char* C_DIRECTORY_ADM = NULL;

void elegirDirectorios(int brokerNumber);
void registrarAgenteEnBrokers(int tipoAgente, long idAgente, int brokerNumber);

void registrarDisponibilidadDeAgente(int tipoAgente, int idBroker);
void verificarGrupoCompleto(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo, int nroBroker);
void enviarMensajeIniciacionLider(int nroGrupo);
bool brokerPerteneceAGrupo(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo, int nroBroker);

int main(int argc, char* argv[]) {
    char buffer[TAM_BUFFER];
    char bufferSocket[TAM_BUFFER];
    char bufferMsgQueue[MSG_BROKER_SIZE];
    ArgumentParser argParser(argc, argv);
    int idSd = 0;
    long idAgente = 0;
    int brokerNumber = 0;
    int idTipoAgente;

    if ( argParser.parseArgument(1, idSd) == -1 ) {
        exit(-1);
    }

    if ( argParser.parseArgument(2, brokerNumber) == -1 ) {
        exit(-1);
    }

    sprintf(buffer, "CanalSalidaBrokerAgente N°%d", brokerNumber);
    Logger::setProcessInformation( buffer );

    SocketStream socketAgente(idSd);
    Logger::logMessage(Logger::COMM, "Canal de Salida conectado");
    
    // Recibe el id los datos del Agente
    if ( socketAgente.receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
        Logger::logMessage(Logger::ERROR, "Error al recibir el id del Agente");
        socketAgente.destroy();
        abort();
    }
    
    std::stringstream ss;
    ss << bufferSocket;
    ss >> idAgente;
    ss >> idTipoAgente;
    
    sprintf(buffer, "Se reciben los datos del agente: %ld - %d",
            idAgente, idTipoAgente);
    Logger::logMessage(Logger::COMM, buffer);

    try {
        // Registro al agente que se registró en los Brokers
        elegirDirectorios( brokerNumber );
        registrarAgenteEnBrokers(idTipoAgente, idAgente, brokerNumber);
    
        IPC::MsgQueue colaBroker("cola Broker");
        colaBroker.getMsgQueue(C_DIRECTORY_BROKER, idTipoAgente);
        
        while ( true ) {
            MsgCanalSalidaBroker mensaje;
            //colaBroker.recv(idAgente, mensaje);
            colaBroker.recv(idAgente, bufferMsgQueue, MSG_BROKER_SIZE);
            memcpy(&mensaje, bufferMsgQueue, sizeof(MsgCanalSalidaBroker));

            sprintf(buffer, "parametros: mtype recibido (idAgente): %ld", 
            mensaje.mtype);
            Logger::logMessage(Logger::COMM, buffer);
            
            
            memcpy(bufferSocket, &mensaje.msg, sizeof(MsgCanalEntradaAgente));
            if (socketAgente.send(bufferSocket, TAM_BUFFER) != TAM_BUFFER) {
                Logger::logMessage(Logger::ERROR, "Error al enviar "
                "mensajes al Agente");
                socketAgente.destroy();
                abort();
            }
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
    
    
    socketAgente.destroy();
    Logger::logMessage(Logger::COMM, "Se destruye el canal.");
    return 0;
}

void elegirDirectorios(int brokerNumber) {
    switch (brokerNumber) {
        case 1:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_1;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_1;
            C_DIRECTORY_ADM = DIRECTORY_ADM_1;
            break;
        case 2:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_2;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_2;
            C_DIRECTORY_ADM = DIRECTORY_ADM_2;
            break;
        case 3:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_3;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_3;
            C_DIRECTORY_ADM = DIRECTORY_ADM_3;
            break;
        case 4:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_4;
            C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_4;
            C_DIRECTORY_ADM = DIRECTORY_ADM_4;
            break;
        default:
            Logger::logMessage(Logger::ERROR, "Error al elegir directorios del Broker");
            abort();
    }
}

void registrarAgenteEnBrokers(int tipoAgente, long idAgente, int brokerNumber) {
    // Primero registro al agente en el propio Broker
    IPC::SharedMemory<DataInfoAgentes> shMemInfoAgentes;
    shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, tipoAgente);
    IPC::Semaphore semMutexShMemInfoAgentes;
    semMutexShMemInfoAgentes.getSemaphore(C_DIRECTORY_INFO_AGENTES,
                                          ID_INFO_AGENTES, AMOUNT_AGENTS);

    semMutexShMemInfoAgentes.wait( tipoAgente - 1 );

    DataInfoAgentes dataInfoAgentes;
    shMemInfoAgentes.read( &dataInfoAgentes );
    dataInfoAgentes.agenteEnBroker[idAgente] = brokerNumber;
    shMemInfoAgentes.write( &dataInfoAgentes );

    semMutexShMemInfoAgentes.signal( tipoAgente - 1 );

    // Luego, se envía a el mensaje de Broadcast a todos los Brokers
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(SERVERS_CONFIG_FILE) );
    cfg->parse();
    int cantidadBrokers = cfg->getConfigFileParam("CantidadBrokers", -1);

    if ( cantidadBrokers == -1 ) {
        Logger::logMessage(Logger::ERROR, "Error al parser parámetro");
        abort();
    }

    // Se crea el mensaje y se envía al CSBB
    TriadaInfoAgente triada;
    triada.idTipoAgente = tipoAgente;
    triada.idAgente = idAgente;
    triada.idBroker = brokerNumber;

    IPC::MsgQueue colaSalidaBrokerBroker;
    colaSalidaBrokerBroker.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);

    MsgCanalEntradaBrokerBroker msgEntrada;
    MsgCanalSalidaBrokerBroker msgSalida;

    msgEntrada.tipoMensaje = MENSAJE_BROADCAST;
    memcpy(msgEntrada.msg, &triada, sizeof(TriadaInfoAgente));
    memcpy(&msgSalida.msg, &msgEntrada, sizeof(MsgCanalEntradaAgente));

    char buffer[255];
    sprintf(buffer, "Cantidad Brokers: %d - BrokerNumber: %d", cantidadBrokers, brokerNumber);
    Logger::logMessage(Logger::IMPORTANT, buffer);
    
    for (int i = 1; i <= cantidadBrokers; ++i) {
        if (i == brokerNumber)
            continue;
        msgSalida.mtype = i;

        sprintf(buffer, "Envía mensaje de broadcast a Broker N°%d", i);
        Logger::logMessage(Logger::IMPORTANT, buffer);

        colaSalidaBrokerBroker.send( msgSalida );
    }

    /* Luego, se escribe en la matriz como tipo agente disponible. */
    registrarDisponibilidadDeAgente(tipoAgente, brokerNumber);
}

void registrarDisponibilidadDeAgente(int tipoAgente, int idBroker)
{
    char buffer[1024];

    IPC::SharedMemory<InformacionGrupoShMemBrokers> shMemInfoGruposShMemBrokers;
    shMemInfoGruposShMemBrokers.getSharedMemory(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS);

    IPC::Semaphore semInfoGruposShMemBrokers;
    semInfoGruposShMemBrokers.getSemaphore(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS, 1);

    InformacionGrupoShMemBrokers infoGrupoShMemBrokers;
    semInfoGruposShMemBrokers.wait();
    shMemInfoGruposShMemBrokers.read(&infoGrupoShMemBrokers);

    // Recorro todos los grupos y verifico si el agente es necesario para cada uno.
    for(int nroGrupo = 0; nroGrupo < CANT_GRUPOS_SHMEM; nroGrupo++)
    {
        if(infoGrupoShMemBrokers.tiposDeAgenteNecesariosPorGrupo[nroGrupo][tipoAgente - 1] != 0)
        {
            // El agente pertence al grupo
            sprintf(buffer, "Registro de agente de tipo %d en grupo %d desde el broker %d (PRESELECCIÓN LÍDER FORÁNEO).",
                    tipoAgente, nroGrupo + ID_PRIMER_GRUPO_SHMEM, idBroker);
            Logger::logMessage(Logger::IMPORTANT, buffer);

            /* Se puede dar que haya más tipos de agentes inscriptos que necesarios (caso vendedores) */
            if(infoGrupoShMemBrokers.tiposDeAgenteRestantesPorGrupo[nroGrupo][tipoAgente - 1] != 0)
            {
                infoGrupoShMemBrokers.tiposDeAgenteRestantesPorGrupo[nroGrupo][tipoAgente - 1]--;
                verificarGrupoCompleto(&infoGrupoShMemBrokers, nroGrupo, idBroker);
            }
        }
    }

    shMemInfoGruposShMemBrokers.write(&infoGrupoShMemBrokers);
    semInfoGruposShMemBrokers.signal();
}

void verificarGrupoCompleto(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo, int nroBroker)
{
    char buffer[1024];

    for(int nroAgente = 0; nroAgente < AMOUNT_AGENTS; nroAgente++)
    {
        int necesarios = infoGrupoShMemBrokers->tiposDeAgenteNecesariosPorGrupo[nroGrupo][nroAgente];
        if (necesarios > 0) {
            int restantes = infoGrupoShMemBrokers->tiposDeAgenteRestantesPorGrupo[nroGrupo][nroAgente];
            if (restantes > 0) {
                return;
            }
        }
    }

    //infoGrupoShMemBrokers->grupoCompleto[nroGrupo] = true;

    if(brokerPerteneceAGrupo(infoGrupoShMemBrokers, nroGrupo, nroBroker)) {
        sprintf(buffer, "Broker %d pertenece a grupo %d que está completo, manda mensaje a algoritmo de líder.", nroBroker, nroGrupo);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        enviarMensajeIniciacionLider(nroGrupo);
    }
    else {
        sprintf(buffer, "Broker %d no inicia su algoritmo de líder en grupo %d completo por no pertenecer a él.", nroBroker, nroGrupo);
        Logger::logMessage(Logger::IMPORTANT, buffer);
    }
}

/* Verifica si un broker dado tiene algún agente conectado que use una memoria compartida
 * que pertenezca al grupo pasado por parámetro. */
bool brokerPerteneceAGrupo(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo, int nroBroker)
{
    IPC::Semaphore semMutexDataInfoAgentes;
    semMutexDataInfoAgentes.getSemaphore(C_DIRECTORY_INFO_AGENTES, ID_INFO_AGENTES, AMOUNT_AGENTS);

    /* Se debe verificar para todo tipo de agente... */
    for(int nroTipoAgente = 0; nroTipoAgente < AMOUNT_AGENTS; nroTipoAgente++)
    {
        /* ...que ese tipo pertenezca al grupo... */
        if(infoGrupoShMemBrokers->tiposDeAgenteNecesariosPorGrupo[nroGrupo][nroTipoAgente] <= 0) {
            continue;
        }

        IPC::SharedMemory<DataInfoAgentes> shMemDataInfoAgentes;
        shMemDataInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, nroTipoAgente + 1);

        DataInfoAgentes dataInfoAgentes;
        semMutexDataInfoAgentes.wait(nroTipoAgente);
        shMemDataInfoAgentes.read(&dataInfoAgentes);
        semMutexDataInfoAgentes.signal(nroTipoAgente);

        /* ...y que tenga un agente conectado al broker pasado. */
        for(int nroAgenteEnBroker = 0; nroAgenteEnBroker < MAX_AMOUNT_AGENTS; nroAgenteEnBroker++)
        {
            if(dataInfoAgentes.agenteEnBroker[nroAgenteEnBroker] == nroBroker)
                return true;
        }
    }

    return false;
}

void enviarMensajeIniciacionLider(int nroGrupo)
{
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Envio mensaje inicializacion para el grupo %d",nroGrupo);
    Logger::logMessage(Logger::DEBUG, buffer);

    // Semaforo de bloqueo de los algoritmo de lider
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE) );
    cfg->parse();
    std::list<int> sharedMemoryListIds = cfg->getParamIntList("shMem");
    int listSize = sharedMemoryListIds.size();
    IPC::Semaphore semaforoLider = IPC::Semaphore("Semaforo Bloqueo Lider");
    semaforoLider.getSemaphore(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER, listSize);

    semaforoLider.signal(nroGrupo);

    /*IPC::MsgQueue colaLider = IPC::MsgQueue("Cola Lider");
    colaLider.getMsgQueue(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER);

    MsgAlgoritmoLider msgAlgoritmo;
    msgAlgoritmo.mtype = nroGrupo + ID_PRIMER_GRUPO_SHMEM;
    msgAlgoritmo.uid = 0;
    msgAlgoritmo.status = INICIAR;
    colaLider.send(msgAlgoritmo);*/
}

