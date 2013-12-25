/* Este Canal es interno entre Brokers a diferencia de CanalEntradaBroker que
 * relaciona un agente con un broker
 */

#include <Common.h>
#include <middlewareCommon.h>
#include <stdlib.h>

#include <Logger/Logger.h>
#include <Socket/SocketConnector.h>
#include <Exceptions/Exception.h>

#include <IPCs/IPCTemplate/SharedMemory.h>
#include <IPCs/Semaphore/Semaphore.h>
#include <IPCs/IPCTemplate/MsgQueue.h>

#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Comunicaciones/Objects/ServersManager.h>

#include <ConfigFileParser/ConfigFileParser.h>

#include <Util.h>

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_ADM = NULL;
static const char* C_DIRECTORY_INFO_AGENTES = NULL;

void elegirDirectorios(int brokerNumber);
int obtenerNroBrokerDeAgente(TipoAgente idTipoAgente, long idAgente);
void registrarDisponibilidadDeAgente(int tipoAgente, int idBroker);
void verificarGrupoCompleto(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo, int nroBroker);
void enviarMensajeIniciacionLider(int nroGrupo);
bool brokerPerteneceAGrupo(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo, int nroBroker);

int main(int argc, char* argv[]) {
    ArgumentParser argParser(argc, argv);
    char buffer[TAM_BUFFER];
    char bufferSocket[TAM_BUFFER];

    int socketDescriptor = 0;
    int brokerNumber = 0;
    int remoteBrokerId = 0;

    if ( argParser.parseArgument(1, socketDescriptor) == -1 ) {
        Logger::logMessage(Logger::ERROR, "ERROR: Parse Argument 1.");
        exit(-1);
    }

    if ( argParser.parseArgument(2, brokerNumber) == -1 ) {
        Logger::logMessage(Logger::ERROR, "ERROR: Parse Argument 2.");
        exit(-1);
    }

    if ( argc == 4 && socketDescriptor == 0 ) {
        if ( argParser.parseArgument(3, remoteBrokerId) == -1 ) {
            Logger::logMessage(Logger::ERROR, "ERROR: Parse Argument 3.");
            exit(-1);
        }
    }

    elegirDirectorios( brokerNumber );
    sprintf(buffer, "CanalEntradaBrokerBroker N°%d - N°%d:", brokerNumber, remoteBrokerId);
    Logger::setProcessInformation(buffer);

    SocketStream* socketBroker = NULL;
    if ( socketDescriptor == 0 ) {
        sprintf(buffer, "Creando conexión con Broker N°%d", remoteBrokerId);
        Logger::logMessage(Logger::DEBUG, buffer);
        // El proceso no fue creado por un servidor, debe conectarse al
        // mismo
        ServersManager serversManager;
        socketBroker = serversManager.connectToBrokerServer(
                "ServidorCanalSalidaBrokerBroker", remoteBrokerId);

        if ( socketBroker == NULL ) {
            abort();
        }

        // Envía al otro extremo del canal su número de Broker
        memcpy(bufferSocket, &brokerNumber, sizeof(int));
        if ( socketBroker->send(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
            Logger::logMessage(Logger::ERROR, "Error al envíar brokerNumber por el Socket");
            socketBroker->destroy();
            abort();
        }
    }
    else {
        // El proceso fue creado por un servidor
        SocketStream aux( socketDescriptor );
        socketBroker = &aux;

        // Recibe el número de Broker con el cual se conectó
        if ( socketBroker->receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
            Logger::logMessage(Logger::ERROR, "Error al recibir brokerNumber por el Socket");
            socketBroker->destroy();
            abort();
        }
        memcpy(& remoteBrokerId, bufferSocket, sizeof(int));
    }

    sprintf(buffer, "CanalEntradaBrokerBroker N°%d - N°%d:", brokerNumber, remoteBrokerId);
    Logger::setProcessInformation(buffer);
    Logger::logMessage(Logger::COMM, "Conexión realizada correctamente");

    // Luego de la etapa de Conexión, el proceso comienza a recibir mensajes de otros
    // Brokers y a procesarlos
    try {
        IPC::Semaphore semUltimoACKRecibido;
        semUltimoACKRecibido.getSemaphore(C_DIRECTORY_BROKER, ID_SEM_TIMEOUT, 4);

        sprintf(buffer, "Obtengo el Sem Timeout con directorio %s, id %d y cantSemáforos 4 (IPC TIMEOUT).", C_DIRECTORY_BROKER, ID_SEM_TIMEOUT);
        Logger::logMessage(Logger::DEBUG, buffer);

        IPC::SharedMemory<ulong> shMemUltimoACKRecibido;
        shMemUltimoACKRecibido.getSharedMemory(C_DIRECTORY_BROKER, ID_SHMEM_TIMEOUT + remoteBrokerId - 1);

        sprintf(buffer, "Obtengo shMem con directorio %s e id %d (IPC TIMEOUT).", C_DIRECTORY_BROKER, ID_SHMEM_TIMEOUT + remoteBrokerId - 1);
        Logger::logMessage(Logger::DEBUG, buffer);

        IPC::MsgQueue msgQueueACK;
        msgQueueACK.getMsgQueue(C_DIRECTORY_BROKER, ID_MSGQUEUE_TIMEOUT);

        while( true ) {
            if ( socketBroker->receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de un Broker");
                socketBroker->destroy();
                abort();
            }
            
            Logger::logMessage(Logger::COMM, "Recibio mensaje");

            // Actúo en función del mensaje recibido
            MsgCanalEntradaBrokerBroker mensaje;
            memcpy(&mensaje, bufferSocket, sizeof(MsgCanalEntradaBrokerBroker));

            if ( mensaje.tipoMensaje == AGENTE_AGENTE ) {
                MsgCanalEntradaBroker msg;
                memcpy(&msg, mensaje.msg, sizeof(MsgCanalEntradaBroker));
                
                
                if (msg.receiverType == AGENTE) {
                    DireccionamientoMsgAgente dirMsgAgente;
                    memcpy(&dirMsgAgente, msg.direccionamiento, sizeof(DireccionamientoMsgAgente));

                    char buffer[TAM_BUFFER];
                    sprintf(buffer, "Recibe mensaje de Agente: idTipoReceptor: %d", dirMsgAgente.idReceiverAgentType);
                    Logger::logMessage(Logger::COMM, buffer);

                    int idBrokerAgente = obtenerNroBrokerDeAgente(dirMsgAgente.idReceiverAgentType, 
                                                                  dirMsgAgente.idReceptor);

                    if ( idBrokerAgente == brokerNumber ) {
                        // TODO: Se reenvia al CanalSalidaBrokerAgente
                        IPC::MsgQueue colaAgente("colaAgente");
                        colaAgente.getMsgQueue(C_DIRECTORY_BROKER, dirMsgAgente.idReceiverAgentType);
                        colaAgente.send(msg.msg);
                    }
                    else {
                        Logger::logMessage(Logger::ERROR, "Flujo invalido - El mensaje no tenia que llegar a este broker");
                    }
                }
                
                
                // TODO:
            }
            else if ( mensaje.tipoMensaje == MEMORIA_AGENTE ) {
                // TODO: hacer este mensaje francisco, no seas choto.... jaja
                // se la da al administrador_Memoria!!!
                
                Logger::logMessage(Logger::COMM, "el mensaje llego del otro broker, se la doy al administrador");
                MsgEntregaMemoriaAdministrador msg;
                memcpy(&msg, mensaje.msg, sizeof(MsgEntregaMemoriaAdministrador));

                IPC::MsgQueue colaAdministrador("colaAdministrador");
                colaAdministrador.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_MEMORIA);
                colaAdministrador.send(msg);

                sprintf(buffer, "Acaba de llegar un mensaje con id %lu (TIMEOUT).", mensaje.msg_id);
                Logger::logMessage(Logger::DEBUG, buffer);

                /* Prueba ACK. */
                int numeroAleatorio = Util::generateRandomNumber(0, 100);
                if(numeroAleatorio == 4)
                {
                    sprintf(buffer, "No se envía un ACK al B%d (TIMEOUT B%d)!", remoteBrokerId, brokerNumber);
                    Logger::logMessage(Logger::IMPORTANT, buffer);
                    continue;
                }

                /* Envía el ACK. */
                MsgCanalEntradaBrokerBroker msgACKEntrada;
                MsgCanalSalidaBrokerBroker msgACKSalida;

                msgACKEntrada.tipoMensaje = MENSAJE_ACK;
                msgACKEntrada.msg_id = mensaje.msg_id;
                msgACKSalida.mtype = remoteBrokerId;
                memcpy(&msgACKSalida.msg, &msgACKEntrada, sizeof(MsgCanalEntradaBrokerBroker));

                IPC::MsgQueue colaCanalSalidaBrokerBroker;
                colaCanalSalidaBrokerBroker.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);
                colaCanalSalidaBrokerBroker.send(msgACKSalida);

                sprintf(buffer, "Envío ACK nro %lu al broker B%d! (TIMEOUT B%d)", mensaje.msg_id, remoteBrokerId, brokerNumber);
                Logger::logMessage(Logger::DEBUG, buffer);
            }
            else if ( mensaje.tipoMensaje == MENSAJE_LIDER ) {
                Logger::logMessage(Logger::COMM, "el mensaje de lider llego del otro broker, se la doy al algoritmo del lider");
                MsgAlgoritmoLider msg;
                memcpy(&msg, mensaje.msg, sizeof(MsgAlgoritmoLider));
                
                IPC::MsgQueue colaLider("colaLider");
                colaLider.getMsgQueue(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER);
                colaLider.send(msg);
            }

            else if(mensaje.tipoMensaje == MENSAJE_ACK)
            {
                sprintf(buffer, "quiere escribir en memoria compartida %d que se recibe el ACK del mensaje con id %lu. (IPC TIMEOUT)", remoteBrokerId - 1, mensaje.msg_id);
                Logger::logMessage(Logger::DEBUG, buffer);

                sprintf(buffer, "accedo a la shMem haciendo p() en el semáforo %d (IPC TIMEOUT)", remoteBrokerId - 1);
                Logger::logMessage(Logger::DEBUG, buffer);

                /* Registra el número de ACK recibido. */
                semUltimoACKRecibido.wait(remoteBrokerId - 1);
                shMemUltimoACKRecibido.write(&mensaje.msg_id);
                semUltimoACKRecibido.signal(remoteBrokerId - 1);

                sprintf(buffer, "Escribe en memoria compartida %d que se recibe el ACK del mensaje con id %lu. (TIMEOUT)", remoteBrokerId - 1, mensaje.msg_id);
                Logger::logMessage(Logger::DEBUG, buffer);

                MsgACK ack;
                ack.mtype = remoteBrokerId;
                ack.msg_id = mensaje.msg_id;
                ack.recepcionOK = true;
                sprintf(buffer, "Canal va a reenviar ACK al CS mediante la cola %d (TIMEOUT)", remoteBrokerId);
                Logger::logMessage(Logger::DEBUG, buffer);
                msgQueueACK.send(ack);
            }

            else if( mensaje.tipoMensaje == MENSAJE_BROADCAST ) {
                // Recibo la información del agente conectado y actualizo
                // la información de Agentes del Broker
                DataInfoAgentes dataInfoAgentes;
                TriadaInfoAgente triada;
                memcpy(&triada, mensaje.msg, sizeof(TriadaInfoAgente));
                
                sprintf(buffer, "Se recibio mensaje broadcast: idAgente: %ld, "
                        "idBroker: %d, idTipoAgente %d", triada.idAgente, 
                        triada.idBroker, triada.idTipoAgente);
                Logger::logMessage(Logger::COMM, buffer);

                IPC::Semaphore semMutexDataInfoAgentes;
                semMutexDataInfoAgentes.getSemaphore(C_DIRECTORY_INFO_AGENTES,
                                                     ID_INFO_AGENTES, AMOUNT_AGENTS);

                IPC::SharedMemory<DataInfoAgentes> shMemDataInfoAgentes;
                shMemDataInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES,
                                                     triada.idTipoAgente);

                semMutexDataInfoAgentes.wait( triada.idTipoAgente - 1);
                shMemDataInfoAgentes.read( &dataInfoAgentes );

                dataInfoAgentes.agenteEnBroker[triada.idAgente] = triada.idBroker;

                shMemDataInfoAgentes.write( &dataInfoAgentes );
                semMutexDataInfoAgentes.signal( triada.idTipoAgente -1 );

                registrarDisponibilidadDeAgente(triada.idTipoAgente, brokerNumber);
            }
        }
    }
    catch( Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        socketBroker->destroy();
        if (socketDescriptor == 0) {
            delete socketBroker;
        }
        abort();
    }

    Logger::logMessage(Logger::COMM, "Destruyendo canal");
    socketBroker->destroy();
    if (socketDescriptor == 0) {
        delete socketBroker;
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

int obtenerNroBrokerDeAgente(TipoAgente idTipoAgente, long idAgente) {
    // chequear que el receptor pertenezca a este broker
    IPC::Semaphore semMutexShMemInfoAgentes;
    IPC::SharedMemory<DataInfoAgentes> shMemInfoAgentes;
    DataInfoAgentes dataInfoAgentes;

    semMutexShMemInfoAgentes.getSemaphore(C_DIRECTORY_INFO_AGENTES, ID_INFO_AGENTES, AMOUNT_AGENTS);
    shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, idTipoAgente);

    semMutexShMemInfoAgentes.wait( idTipoAgente - 1 );
    shMemInfoAgentes.read( &dataInfoAgentes );

    semMutexShMemInfoAgentes.signal( idTipoAgente - 1 );
    
    return dataInfoAgentes.agenteEnBroker[idAgente];
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

    if(brokerPerteneceAGrupo(infoGrupoShMemBrokers, nroGrupo, nroBroker))
    {
        sprintf(buffer, "Broker %d pertenece a grupo %d que está completo, manda mensaje a algoritmo de líder.", nroBroker, nroGrupo);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        enviarMensajeIniciacionLider(nroGrupo);
    }

    else
    {
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
        if(infoGrupoShMemBrokers->tiposDeAgenteNecesariosPorGrupo[nroGrupo][nroTipoAgente] <= 0)
            continue;

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
    msgAlgoritmo.mtype = nroGrupo+ID_PRIMER_GRUPO_SHMEM;
    msgAlgoritmo.uid = 0;
    msgAlgoritmo.status = INICIAR;
    colaLider.send(msgAlgoritmo);*/
}
