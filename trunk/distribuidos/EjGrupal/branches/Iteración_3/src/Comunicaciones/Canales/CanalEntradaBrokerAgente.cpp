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

int main(int argc, char* argv[]) {
    Logger::setProcessInformation("CanalEntradaBrokerAgente:");
    //char buffer[TAM_BUFFER];
    char bufferSocket[TAM_BUFFER];
    CommunicationsUtil util;
    int idSd = 0;

    if ( util.parseArgs(argc, argv, idSd) == -1 ) {
        exit(-1);
    }

    SocketStream socketAgente(idSd);
    Logger::logMessage(Logger::COMM, "Canal de Entrada conectado");
    
    try {
        IPC::MsgQueue colaAgente;
        while ( true ) {
            if (socketAgente.receive(bufferSocket, TAM_BUFFER) != TAM_BUFFER) {
                Logger::logMessage(Logger::ERROR, "Error al recibir "
                "mensajes del Agente");
                socketAgente.destroy();
                abort();
            }
            
            MsgCanalEntradaBroker mensaje;
            memcpy(&mensaje, bufferSocket, sizeof(MsgCanalEntradaBroker));

            
            
            /*msg_pedido_t pedido;
            memcpy(&pedido, mensaje.msg.msg.msg.msg, sizeof(msg_pedido_t));

            sprintf(buffer, "MsgPedidoT: %d - %d", pedido.tipo, pedido.pedido.tipoProducto);
            Logger::logMessage(Logger::IMPORTANT, buffer);*/
            
            //sprintf(buffer, "parametros mensaje: mtype del siguiente salto: %ld, idReceptor %ld, idTipoReceptor %d ", mensaje.msg.mtype, mensaje.idReceptor, mensaje.idTipoReceptor);
            //Logger::logMessage(Logger::COMM, buffer);
            
            
            
            
            if (mensaje.receiverType == AGENTE) {
                DireccionamientoMsgAgente dirMsgAgente;
                memcpy(&dirMsgAgente, mensaje.direccionamiento, sizeof(DireccionamientoMsgAgente));
                
                char buffer[TAM_BUFFER];
                sprintf(buffer, "Recibe mensaje de Agente: idTipoReceptor: %d", dirMsgAgente.idReceiverAgentType);
                Logger::logMessage(Logger::COMM, buffer);
                
                colaAgente.getMsgQueue(DIRECTORY_BROKER, dirMsgAgente.idReceiverAgentType);
            } else if (mensaje.receiverType == ADMINISTRADOR_MEMORIA) {
                DireccionamientoMsgAdministrador dirMsgAdm;
                memcpy(&dirMsgAdm, mensaje.direccionamiento, sizeof(DireccionamientoMsgAdministrador));                
                
                colaAgente.getMsgQueue(DIRECTORY_BROKER, dirMsgAdm.idMsgAdmType);
                
                if (dirMsgAdm.idMsgAdmType == ID_TIPO_PEDIDO_MEMORIA) {
                    Logger::logMessage(Logger::COMM, "Pedido memoria");

                    IPC::SharedMemory<int> contadoraSharedMemory = IPC::SharedMemory<int>("Contadora Pedidos Sh Mem");
                    contadoraSharedMemory.getSharedMemory(DIRECTORY_ADM, dirMsgAdm.idMemory); 
                    IPC::Semaphore semaforoContadora = IPC::Semaphore("Semaforo Contadora Pedidos");
                    semaforoContadora.getSemaphore(DIRECTORY_ADM, dirMsgAdm.idMemory, 1);
                    semaforoContadora.wait();
                    int cantidad;
                    contadoraSharedMemory.read(&cantidad);
                    cantidad++;
                    contadoraSharedMemory.write(&cantidad);
                    semaforoContadora.signal();
                }
            } else if (mensaje.receiverType == BROKER) {
                Logger::logMessage(Logger::ERROR, "no tenia que entrar aca!!!");
                // TODO 
            } else {
                Logger::logMessage(Logger::ERROR, "mensaje mal formado - el receiverType no es valido");
            }
            
            colaAgente.send(mensaje.msg);
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
    
