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

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_ADM = NULL;
static const char* C_DIRECTORY_INFO_AGENTES = NULL;

void elegirDirectorios(int brokerNumber);
int obtenerNroBrokerDeAgente(TipoAgente idTipoAgente, long idAgente);
void registrarDisponibilidadDeAgente(int tipoAgente, int idBroker);
void verificarGrupoCompleto(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo);
void enviarMensajeIniciacionLider(int nroGrupo);

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
        while( true ) {
            if ( socketBroker->receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER ) {
                Logger::logMessage(Logger::ERROR, "Error al recibir mensaje de un Broker");
                socketBroker->destroy();
                abort();
            }
            
            Logger::logMessage(Logger::IMPORTANT, "Recibio mensaje");

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
                
                Logger::logMessage(Logger::IMPORTANT, "el mensaje llego del otro broker, se la doy al administrador");
                MsgEntregaMemoriaAdministrador msg;
                memcpy(&msg, mensaje.msg, sizeof(MsgEntregaMemoriaAdministrador));

                IPC::MsgQueue colaAdministrador("colaAdministrador");
                colaAdministrador.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_MEMORIA);
                colaAdministrador.send(msg);
            }
            else if ( mensaje.tipoMensaje == MENSAJE_LIDER ) {
                Logger::logMessage(Logger::IMPORTANT, "el mensaje de lider llego del otro broker, se la doy al algoritmo del lider");
                MsgAlgoritmoLider msg;
                memcpy(&msg, mensaje.msg, sizeof(MsgAlgoritmoLider));
                
                IPC::MsgQueue colaLider("colaLider");
                colaLider.getMsgQueue(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER);
                colaLider.send(msg);
            }
            else if ( mensaje.tipoMensaje == AGENTE_CONECTADO ) {
                // TODO:
            }
            else if ( mensaje.tipoMensaje == MENSAJE_BROADCAST ) {
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

                registrarDisponibilidadDeAgente(triada.idTipoAgente, triada.idBroker);
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
            sprintf(buffer, "Se registra un agente de tipo %d (PRESELECCIÓN LÍDER).", tipoAgente);
            Logger::logMessage(Logger::IMPORTANT, buffer);
            //infoGrupoShMemBrokers.tiposDeAgenteRestantePorGrupo[nroGrupo][tipoAgente - 1]--;
            infoGrupoShMemBrokers.tiposDeAgenteRestantePorGrupo[nroGrupo][tipoAgente - 1] = idBroker;
            verificarGrupoCompleto(&infoGrupoShMemBrokers, nroGrupo);
        }
    }

    shMemInfoGruposShMemBrokers.write(&infoGrupoShMemBrokers);
    semInfoGruposShMemBrokers.signal();
}

void verificarGrupoCompleto(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int nroGrupo)
{
    for(int nroAgente = 0; nroAgente < AMOUNT_AGENTS; nroAgente++)
    {
        int necesario = infoGrupoShMemBrokers->tiposDeAgenteNecesariosPorGrupo[nroGrupo][nroAgente];
        if (necesario != 0) {
            // Si es necesario, verifico si esta en algún broker
            int broker = infoGrupoShMemBrokers->tiposDeAgenteRestantePorGrupo[nroGrupo][nroAgente];
            if (broker == 0) {
                // Si es necesario, y no esta en ningun broker, el grupo no esta completo
                return;
            }
        }
    }

    if (! infoGrupoShMemBrokers->grupoCompleto[nroGrupo]) {
        // Solo inicio el algoritmo del lider si el grupo no estaba previamente completo
        infoGrupoShMemBrokers->grupoCompleto[nroGrupo] = true;
        //enviarMensajeIniciacionLider(nroGrupo);
    }
}

void enviarMensajeIniciacionLider(int nroGrupo)
{
    IPC::MsgQueue colaLider = IPC::MsgQueue("Cola Lider");
    colaLider.getMsgQueue(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER);

    MsgAlgoritmoLider msgAlgoritmo;
    msgAlgoritmo.mtype = static_cast<long>(nroGrupo);
    msgAlgoritmo.status = INICIAR;
    colaLider.send(msgAlgoritmo);
}
