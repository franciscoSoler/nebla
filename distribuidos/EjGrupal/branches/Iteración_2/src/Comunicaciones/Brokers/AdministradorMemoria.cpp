/* 
 * File:   AdministradorMemoria.cpp
 * Author: nebla
 * 
 * Created on December 7, 2013, 11:40 PM
 */

#include "../../Common.h"
#include "../../middlewareCommon.h"

#include "../Objects/ArgumentParser.h"

#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCTemplate/MsgQueue.h"
#include "../../IPCs/IPCTemplate/SharedMemory.h"

#include "../Objects/CommPacketWrapper.h"

int main(int argc, char* argv[]) {
    
    Logger::setProcessInformation("Administrador Memoria:");
    
    char buffer[TAM_BUFFER];
    ArgumentParser argParser(argc, argv);
    
    int idMemoria;

    if ( argParser.parseArgument(1, idMemoria) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 1");
        exit(-1);
    }
    
    sprintf(buffer, "Administrador Memoria: %d",idMemoria);
    Logger::setProcessInformation(buffer);

    try {
        // Obtengo la cola por la cual recibo la memoria compartida
        IPC::MsgQueue colaMemoria = IPC::MsgQueue("Cola Memoria");
        colaMemoria.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_MEMORIA);

        // Obtengo la cola por la cual recibo los pedidos por memoria compartida
        IPC::MsgQueue colaPedidosMemoria = IPC::MsgQueue("Cola Pedidos Memoria");
        colaPedidosMemoria.getMsgQueue(DIRECTORY_BROKER, ID_TIPO_PEDIDO_MEMORIA);

        // Obtengo la memoria compartida contadora
        IPC::SharedMemory<int> contadoraSharedMemory = IPC::SharedMemory<int>("Contadora Pedidos Sh Mem");
        contadoraSharedMemory.getSharedMemory(DIRECTORY_ADM, idMemoria);
        IPC::Semaphore semaforoContadora = IPC::Semaphore("Semaforo Contadora Pedidos");
        semaforoContadora.getSemaphore(DIRECTORY_ADM, idMemoria,1);

        // Obtengo la memoria compartida con el siguiente broker
        IPC::SharedMemory<int> siguienteSharedMemory = IPC::SharedMemory<int>("Siguiente Broker Sh Mem");
        siguienteSharedMemory.getSharedMemory(DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
        IPC::Semaphore semaforoSiguiente = IPC::Semaphore("Semaforo Siguiente Broker");
        semaforoSiguiente.getSemaphore(DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE, 1);

        char bufferMsgQueue[MSG_BROKER_SIZE];
        //char directorioIPC[DIR_FIXED_SIZE];
        //int identificadorIPC;
        int cantidad = 0;

        while (true) {
            // Recibo el token con la memoria compartida
            MsgEntregaMemoriaAdministrador mensajeMemoria;
            colaMemoria.recv(idMemoria, bufferMsgQueue, MSG_BROKER_SIZE);
            memcpy(&mensajeMemoria, bufferMsgQueue, sizeof(MsgEntregaMemoriaAdministrador));

            semaforoContadora.wait();
            contadoraSharedMemory.read(&cantidad);
            semaforoContadora.signal();

            for (int i = 0; i < cantidad; ++i) {
                // Obtengo un pedido por la memoria
                MsgPedidoMemoriaAdministrador mensajePedido;
                colaPedidosMemoria.recv(idMemoria, bufferMsgQueue, MSG_BROKER_SIZE);
                memcpy(&mensajePedido, bufferMsgQueue, sizeof(MsgPedidoMemoriaAdministrador));

                // Envio a la cola del agente que realizo el pedido, la memoria compartida
                CommPacketWrapper wrapper;

                //==== DIRETORIO IPC e IDENTIFICADOR IPC NO ESTAN DEFINIDOS, ESTO EXPLOTA!!====/
                wrapper.setDirIPC(DIRECTORY_SEM);
                wrapper.setIdDirIPC(ID_COMM_SEM_ENTRADA);
                wrapper.setReceiverId(mensajePedido.idReceptor);
                wrapper.setReceiverType(mensajePedido.idTipoReceptor);

                MsgCanalSalidaBroker msgSalida;
                wrapper.createPacketReplyShMem(msgSalida, mensajeMemoria.memoria);

                IPC::MsgQueue colaAgente = IPC::MsgQueue("Cola Agente");
                colaAgente.getMsgQueue(DIRECTORY_BROKER, mensajePedido.idTipoReceptor);
                colaAgente.send(msgSalida);

                // Espero que el agente devuelva la memoria compartida
                colaMemoria.recv(idMemoria, bufferMsgQueue, MSG_BROKER_SIZE);
                memcpy(&mensajeMemoria, bufferMsgQueue, sizeof(MsgEntregaMemoriaAdministrador));
            }

            // Le envio la memoria al siguietne broker, por ahora se vuelve a enviar a la cola de entrada del administrador
            memcpy(bufferMsgQueue, &mensajeMemoria, sizeof(MsgEntregaMemoriaAdministrador));
            colaMemoria.send(mensajeMemoria);
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }

    return 0;
}

