#include <iostream>
#include <string.h>
#include <assert.h>

#include <Logger/Logger.h>
#include <Common.h>
#include <IPCs/IPCTemplate/MsgQueue.h>
#include <IPCs/Semaphore/Semaphore.h>

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/CommunicationsUtil.h>
#include <Comunicaciones/Objects/ArgumentParser.h>
#include <Socket/SocketStream.h>

#include "middlewareCommon.h"
#include "SharedMemory.h"

/* Esto sólo lo vamos a utilizar para poder correr el sistema en una PC
 */

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_ADM = NULL;
static const char* C_DIRECTORY_INFO_AGENTES = NULL;

void elegirDirectorios(int brokerNumber);
int obtenerNroBrokerDeAgente(TipoAgente idTipoAgente, long idAgente);

int main(int argc, char* argv[]) {
    char buffer[TAM_BUFFER];
    char bufferSocket[TAM_BUFFER];
    ArgumentParser argParser(argc, argv);
    int idSd = 0;
    int brokerNumber = 0;

    if ( argParser.parseArgument(1, idSd) == -1 ) {
        exit(-1);
    }

    if ( argParser.parseArgument(2, brokerNumber) == -1 ) {
        exit(-1);
    }

    sprintf(buffer, "CanalEntradaBrokerAgente N°%d", brokerNumber);
    Logger::setProcessInformation( buffer );

    SocketStream socketAgente(idSd);
    Logger::logMessage(Logger::COMM, "Canal de Entrada conectado");

    elegirDirectorios( brokerNumber );
    
    try {
        while ( true ) {
            if (socketAgente.receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER) {
                Logger::logMessage(Logger::ERROR, "Error al recibir "
                "mensajes del Agente");
                socketAgente.destroy();
                abort();
            }
            
            MsgCanalEntradaBroker mensaje;
            memcpy(&mensaje, bufferSocket, sizeof(MsgCanalEntradaBroker));
            
            
            if (mensaje.receiverType == AGENTE) {
                DireccionamientoMsgAgente dirMsgAgente;
                memcpy(&dirMsgAgente, mensaje.direccionamiento, sizeof(DireccionamientoMsgAgente));

                char buffer[TAM_BUFFER];
                sprintf(buffer, "Recibe mensaje de Agente: idTipoReceptor: %d", dirMsgAgente.idReceiverAgentType);
                Logger::logMessage(Logger::COMM, buffer);

                int idBrokerAgente = obtenerNroBrokerDeAgente(dirMsgAgente.idReceiverAgentType, 
                                                              dirMsgAgente.idReceptor);

                if ( idBrokerAgente == brokerNumber ) {
                    // TODO: Se reenvia al CanalSalidaBrokerAgente
                    IPC::MsgQueue colaAgente("colaAgente");
                    colaAgente.getMsgQueue(C_DIRECTORY_BROKER, dirMsgAgente.idReceiverAgentType);
                    colaAgente.send(mensaje.msg);
                }
                else {
                    MsgCanalEntradaBrokerBroker msgEntrada;
                    msgEntrada.tipoMensaje = AGENTE_AGENTE;
                    memcpy(msgEntrada.msg, &mensaje, sizeof(MsgCanalEntradaBroker));

                    MsgCanalSalidaBrokerBroker msgSalida;
                    msgSalida.mtype = brokerNumber;
                    memcpy(&msgSalida.msg, &msgEntrada, sizeof(MsgCanalEntradaBrokerBroker));

                    IPC::MsgQueue colaCanalSalidaBrokerBroker("colaCanalSalidaBrokerBroker");
                    colaCanalSalidaBrokerBroker.send(msgSalida);
                }
            }
            else if (mensaje.receiverType == ADMINISTRADOR_MEMORIA) {                    
                DireccionamientoMsgAdministrador dirMsgAdm;
                memcpy(&dirMsgAdm, mensaje.direccionamiento, sizeof(DireccionamientoMsgAdministrador));                

                IPC::MsgQueue colaAdministrador("colaAdministrador");
                colaAdministrador.getMsgQueue(C_DIRECTORY_BROKER, dirMsgAdm.idMsgAdmType);

                if (dirMsgAdm.idMsgAdmType == ID_TIPO_PEDIDO_MEMORIA) {
                    Logger::logMessage(Logger::COMM, "Pedido memoria");

                    IPC::SharedMemory<int> contadoraSharedMemory = IPC::SharedMemory<int>("Contadora Pedidos Sh Mem");
                    contadoraSharedMemory.getSharedMemory(C_DIRECTORY_ADM, dirMsgAdm.idMemory);
                    IPC::Semaphore semaforoContadora = IPC::Semaphore("Semaforo Contadora Pedidos");
                    semaforoContadora.getSemaphore(C_DIRECTORY_ADM, dirMsgAdm.idMemory, 1);
                    semaforoContadora.wait();
                    int cantidad;
                    contadoraSharedMemory.read(&cantidad);
                    cantidad++;
                    contadoraSharedMemory.write(&cantidad);
                    semaforoContadora.signal();
                }
                colaAdministrador.send(mensaje.msg);
            } 
            else if (mensaje.receiverType == BROKER) {
                Logger::logMessage(Logger::ERROR, "Flujo inválido");
                // TODO 
            } 
            else {
                Logger::logMessage(Logger::ERROR, "mensaje mal formado - el receiverType no es valido");
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
    
