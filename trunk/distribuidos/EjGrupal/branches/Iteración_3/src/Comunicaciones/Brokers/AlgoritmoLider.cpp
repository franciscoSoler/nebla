/*
 * File:   AlgoritmoLider.cpp
 * Author: nebla
 *
 * Created on December 13, 2013, 02:40 PM
 */

#include "../../Common.h"
#include "../../middlewareCommon.h"

#include "../Objects/ArgumentParser.h"

#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCTemplate/MsgQueue.h"
#include "../../IPCs/IPCTemplate/SharedMemory.h"

#include "../Objects/CommPacketWrapper.h"

int main(int argc, char* argv[]) {

    Logger::setProcessInformation("Algoritmo Lider:");

    char buffer[TAM_BUFFER];
    ArgumentParser argParser(argc, argv);

    int idBroker;
    int idGrupo;

    if ( argParser.parseArgument(1, idBroker) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 1");
        exit(-1);
    }

    if ( argParser.parseArgument(2, idGrupo) == -1 ) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 2");
        exit(-1);
    }

    sprintf(buffer, "Algoritmo Lider Nº%d:",idGrupo);
    Logger::setProcessInformation(buffer);

    Logger::logMessage(Logger::DEBUG, "Algoritmo del lider iniciado satisfactoriamente");

    try {
        // Obtengo la cola por la cual recibo los mensajes del algoritmo
        IPC::MsgQueue colaLider = IPC::MsgQueue("Cola Lider");
        colaLider.getMsgQueue(DIRECTORY_BROKER, MENSAJE_LIDER);

        // Obtengo la cola por la cual envio los mensajes al siguiente en el anillo
        IPC::MsgQueue colaBrokers = IPC::MsgQueue("Cola Bokers");
        colaBrokers.getMsgQueue(DIRECTORY_BROKER, );

        // Obtengo la memoria compartida con el siguiente broker
        IPC::SharedMemory<int> siguienteSharedMemory("Siguiente Broker ShMem");
        siguienteSharedMemory.createSharedMemory(DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
        Logger::logMessage(Logger::COMM, "shMem SiguienteBroker creado");

        IPC::Semaphore semaforoSiguiente = IPC::Semaphore("Semaforo Siguiente Broker");
        semaforoSiguiente.createSemaphore(DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE, 1);
        Logger::logMessage(Logger::COMM, "Semaforo shMem SiguienteBroker creado");

        // Obtengo la cantidad todal de grupos
        std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE) );
        cfg->parse();
        std::list<int> sharedMemoryListIds = cfg->getParamIntList("shMem");
        int cantGrupos = sharedMemoryListIds.size();

        IPC::Semaphore semaforoLider = IPC::Semaphore("Semaforo Bloqueo Algoritmo Lider");
        semaforoLider.createSemaphore(DIRECTORY_BROKER, ID_ALGORITMO_LIDER, cantGrupos);
        Logger::logMessage(Logger::COMM, "Semaforo Algoritmo lider creado");


        while (true) {
            // Me bloqueo esperando que deba iniciar el algoritmo
            semaforoLider.wait(idGrupo - 1);

            bool hayLider = false;

            int siguiente;
            semaforoSiguiente.wait();
            siguienteSharedMemory.read(siguiente);
            semaforoSiguiente.signal();

            // Lo primero que hago es enviar mi id
            MsgAlgoritmoLider msgAlgoritmo;
            msgAlgoritmo.status = DESCONOCIDO;
            msgAlgoritmo.uid = idBroker;
            msgAlgoritmo.mtype = idGrupo;

            //TODO: Encapsular el mensaje para enviarlo al siguiente broker

            while (! hayLider) {
                char bufferMsgQueue[MSG_BROKER_SIZE];

                // Espero un mensaje del broker anterior
                colaLider.recv(idGrupo, bufferMsgQueue, MSG_BROKER_SIZE);
                memcpy(&msgAlgoritmo, bufferMsgQueue, sizeof(MsgAlgoritmoLider));

                if (msgAlgoritmo.status == DESCONOCIDO) {
                    if (msgAlgoritmo.uid == idBroker) {
                        // Me llego un mensaje con mi uid, por lo tanto soy el lider.
                        msgAlgoritmo.status = LIDER;
                        msgAlgoritmo.uid = idBroker;
                        msgAlgoritmo.mtype = idGrupo;

                        // Encapsulo y reenvio al siguiente
                    }
                    else if (msgAlgoritmo.uid > idBroker) {
                        // Me llego un mensaje con un uid mayor, por lo tanto lo debo reenviar

                    }
                    else {
                        // Me llego un mensajes con un uid menor, por lo tanto se ignora el mensaje
                    }
                }
                else if (msgAlgoritmo.status == LIDER) {
                    // Marcar en una memoria compartida quien es el nuevo lider.

                    if (msgAlgoritmo.uid != siguiente) {
                        // Si el siguiente broker es el lider, no hace falta mandarle un mensaje

                    }
                }
        }


    }

    return 0;
}

