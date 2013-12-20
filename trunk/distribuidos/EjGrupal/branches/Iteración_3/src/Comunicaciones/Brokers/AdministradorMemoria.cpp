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

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_ADM = NULL;

void elegirDirectorios(int brokerNumber);
void establecerSiguiente(int nroBroker, int nroGrupo);

int main(int argc, char* argv[]) {
    
    Logger::setProcessInformation("Administrador Memoria:");
    
    char buffer[TAM_BUFFER];
    int brokerNumber = 0;
    ArgumentParser argParser(argc, argv);
    
    int idMemoria;

    if ( argParser.parseArgument(1, idMemoria) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 1");
        exit(-1);
    }

    if ( argParser.parseArgument(2, brokerNumber) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 2");
        exit(-1);
    }

    sprintf(buffer, "Administrador Memoria Nº%d - Broker N°%d:",idMemoria, brokerNumber);
    Logger::setProcessInformation(buffer);
    Logger::logMessage(Logger::DEBUG, "Administrador creado satisfactoriamente");

    elegirDirectorios( brokerNumber );

    try {
        // Obtengo la cola por la cual recibo la memoria compartida
        IPC::MsgQueue colaMemoria = IPC::MsgQueue("Cola Memoria");
        colaMemoria.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_MEMORIA);

        // Obtengo la cola por la cual recibo los pedidos por memoria compartida
        IPC::MsgQueue colaPedidosMemoria = IPC::MsgQueue("Cola Pedidos Memoria");
        colaPedidosMemoria.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_PEDIDO_MEMORIA);

        // Obtengo la cola por la cual envio los mensajes al siguiente en el anillo
        IPC::MsgQueue colaBrokers = IPC::MsgQueue("Cola Bokers");
        colaBrokers.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);

        // Obtengo la memoria compartida contadora
        IPC::SharedMemory<int> contadoraSharedMemory = IPC::SharedMemory<int>("Contadora Pedidos Sh Mem");
        contadoraSharedMemory.getSharedMemory(C_DIRECTORY_ADM, idMemoria);
        IPC::Semaphore semaforoContadora = IPC::Semaphore("Semaforo Contadora Pedidos");
        semaforoContadora.getSemaphore(C_DIRECTORY_ADM, idMemoria,1);

        /* Obtengo la memoria compartida que indica el siguiente broker. */
        IPC::SharedMemory<int> siguienteSharedMemory = IPC::SharedMemory<int>("Siguiente Broker Sh Mem");
        siguienteSharedMemory.getSharedMemory(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
        IPC::Semaphore semaforoSiguiente = IPC::Semaphore("Semaforo Siguiente Broker");
        semaforoSiguiente.getSemaphore(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE, 1);

        char bufferMsgQueue[MSG_BROKER_SIZE];
        //char directorioIPC[DIR_FIXED_SIZE];
        //int identificadorIPC;
        int cantidad = 0;
        int cantidadCero = 0;

        while (true) {
            // Recibo el token con la memoria compartida
            MsgEntregaMemoriaAdministrador mensajeMemoria;
            colaMemoria.recv(idMemoria, bufferMsgQueue, MSG_BROKER_SIZE);
            memcpy(&mensajeMemoria, bufferMsgQueue, sizeof(MsgEntregaMemoriaAdministrador));
            // Saco este mensaje porque sino el Log se llena de basura por culpa del Polling
            // Logger::logMessage(Logger::DEBUG, "Recibe mensaje de ColaMemoria");

            semaforoContadora.wait();
            contadoraSharedMemory.read(&cantidad);            
            contadoraSharedMemory.write(&cantidadCero);
            semaforoContadora.signal();

            sprintf(buffer, "Tengo %d pedidos de memoria", cantidad);
            Logger::logMessage(Logger::IMPORTANT, buffer);

            for (int i = 0; i < cantidad; ++i) {
                // Obtengo un pedido por la memoria
                MsgPedidoMemoriaAdministrador mensajePedido;
                colaPedidosMemoria.recv(idMemoria, bufferMsgQueue, MSG_BROKER_SIZE);
                memcpy(&mensajePedido, bufferMsgQueue, sizeof(MsgPedidoMemoriaAdministrador));
                Logger::logMessage(Logger::DEBUG, "Recibe mensaje de ColaPedidoMemoria");

                // Envio a la cola del agente que realizo el pedido, la memoria compartida
                CommPacketWrapper wrapper;

                wrapper.setDirIPC(DIRECTORY_SEM);
                wrapper.setIdDirIPC(mensajePedido.idTipoEmisor);
                //wrapper.setReceiverId(mensajePedido.idReceptor);
                wrapper.setReceiverId(mensajePedido.idEmisor);
                //wrapper.setReceiverType(mensajePedido.idTipoEmisor);

                MsgCanalSalidaBroker msgSalida;
                wrapper.createPacketReplyShMem(msgSalida, mensajeMemoria.memoria);

                IPC::MsgQueue colaAgente = IPC::MsgQueue("Cola Agente");
                colaAgente.getMsgQueue(C_DIRECTORY_BROKER, mensajePedido.idTipoEmisor);
                colaAgente.send(msgSalida);

                // Espero que el agente devuelva la memoria compartida
                colaMemoria.recv(idMemoria, bufferMsgQueue, MSG_BROKER_SIZE);
                memcpy(&mensajeMemoria, bufferMsgQueue, sizeof(MsgEntregaMemoriaAdministrador));
            }
            
            // Obtengo el siguiente broker del anillo
            int siguiente;
            semaforoSiguiente.wait();
            siguienteSharedMemory.read(&siguiente);
            semaforoSiguiente.signal();

            sprintf(buffer, "No realizo mas pedidos, envio shMem al siguiente broker: %d", siguiente);
            Logger::logMessage(Logger::IMPORTANT, buffer);
            
            // Se pone este sleep para que el token no avance muy rapido cuando no hay pedidos
            // del mismo
            if ( not cantidad ) {
                sleep( 1 );
            }

            if (siguiente == brokerNumber) {
                // WARNING: Agrego un sleep para que si no hay mensajes, no se quede en un busy wait!!
                // El siguiente broker soy yo mismo, por lo tanto, "me reenvio" la memoria.
                Logger::logMessage(Logger::IMPORTANT, "estoy autoenviando la shMem");
                memcpy(bufferMsgQueue, &mensajeMemoria, sizeof(MsgEntregaMemoriaAdministrador));
                colaMemoria.send(bufferMsgQueue, MSG_BROKER_SIZE);
            }
            else {
                MsgCanalEntradaBrokerBroker msgEntrada;
                memcpy(msgEntrada.msg, &mensajeMemoria, sizeof(MsgEntregaMemoriaAdministrador));
                msgEntrada.tipoMensaje = MEMORIA_AGENTE;

                MsgCanalSalidaBrokerBroker msgSalida;
                msgSalida.mtype = siguiente;
                memcpy(&msgSalida.msg, &msgEntrada, sizeof(MsgCanalEntradaBrokerBroker));
                // Le envio la memoria al siguietne broker, por ahora se vuelve a enviar a la cola de entrada del administrador
                //memcpy(bufferMsgQueue, &msgSalida, sizeof(MsgCanalSalidaBrokerBroker));
                //colaBrokers.send(bufferMsgQueue, MSG_BROKER_SIZE);
                colaBrokers.send(msgSalida);
            }
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }

    return 0;
}

void elegirDirectorios(int brokerNumber) {
    switch (brokerNumber) {
        case 1:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_1;
            C_DIRECTORY_ADM = DIRECTORY_ADM_1;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_1;
            break;
        case 2:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_2;
            C_DIRECTORY_ADM = DIRECTORY_ADM_2;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_2;
            break;
        case 3:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_3;
            C_DIRECTORY_ADM = DIRECTORY_ADM_3;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_3;
            break;
        case 4:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_4;
            C_DIRECTORY_ADM = DIRECTORY_ADM_4;
            // C_DIRECTORY_INFO_AGENTES = DIRECTORY_INFO_AGENTES_4;
            break;
        default:
            Logger::logMessage(Logger::ERROR, "Error al elegir directorios del Broker");
            abort();
    }
}
