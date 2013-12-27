/*
 * File:   AlgoritmoLider.cpp
 * Author: nebla
 *
 * Created on December 13, 2013, 02:40 PM
 */

#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory>
#include <sstream>

#include <Common.h>
#include <middlewareCommon.h>

#include "../Objects/ArgumentParser.h"

#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCTemplate/MsgQueue.h"
#include "../../IPCs/IPCTemplate/SharedMemory.h"

#include "../Objects/CommPacketWrapper.h"

#include <ConfigFileParser/ConfigFileParser.h>

#include <API/Objects/DataSM_R11_R14.h>
#include <API/Objects/DataSM_R14_R16.h>

#include <Comunicaciones/Objects/ArgumentParser.h>

static const char* C_DIRECTORY_BROKER = NULL;
static const char* C_DIRECTORY_ADM = NULL;
static const char* C_DIRECTORY_INFO_AGENTES = NULL;

void enviarMensajeAlSiguiente(MsgAlgoritmoLider msg, int idBroker, int idGrupo);
void elegirDirectorios(int brokerNumber);
void iniciarMemoria(int idGrupo);
int obtenerSiguiente(int idBroker, int idGrupo);

/* Rearmado de anillo. */
void darBrokerDeBaja(int nroBroker);
void eliminarAgenteDeGrupo(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int idTipoAgente);


int main(int argc, char* argv[]) {
    Logger::setProcessInformation("Algoritmo Lider:");

    char buffer[TAM_BUFFER];
    ArgumentParser argParser(argc, argv);

    int idBroker;
    int idGrupo;

    if (argParser.parseArgument(1, idGrupo) == -1) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 1");
        exit(-1);
    }

    if (argParser.parseArgument(2, idBroker) == -1) {
        Logger::logMessage(Logger::COMM, "ERROR: parseArgument 2");
        exit(-1);
    }

    sprintf(buffer, "Algoritmo Lider Nº%d - Broker N°%d:", idGrupo, idBroker);
    Logger::setProcessInformation(buffer);

    Logger::logMessage(Logger::DEBUG, "Iniciado satisfactoriamente");
   
    elegirDirectorios( idBroker );

    // Semaforo de bloqueo de los algoritmo de lider
    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE) );
    cfg->parse();
    std::list<int> sharedMemoryListIds = cfg->getParamIntList("shMem");
    int listSize = sharedMemoryListIds.size();

    IPC::Semaphore semaforoLider = IPC::Semaphore("Semaforo Bloqueo Lider");
    semaforoLider.getSemaphore(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER, listSize);

    // Obtengo la cola por la cual recibo los mensajes del algoritmo
    IPC::MsgQueue colaLider = IPC::MsgQueue("Cola Lider");
    colaLider.getMsgQueue(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER);

    try {
        while (true) {
            // Me bloqueo esperando que deba iniciar el algoritmo
            MsgAlgoritmoLider msgAlgoritmo;

            /*char bufferMsgQueue[MSG_BROKER_SIZE];
            colaLider.recv(idGrupo, bufferMsgQueue, MSG_BROKER_SIZE);
            memcpy(&msgAlgoritmo, bufferMsgQueue, sizeof (MsgAlgoritmoLider));

            if (msgAlgoritmo.status != INICIAR) {
                // Para desbloquear el algoritmo del lider el primer mensage debe ser de INICIAR
                Logger::logMessage(Logger::ERROR, "Se esta intentando iniciar el algoritmo del lider con un mensaqe invalido.");
                abort();
            }*/
            semaforoLider.wait(idGrupo - ID_PRIMER_GRUPO_SHMEM);
            sprintf(buffer, "Se inicia el algoritmo");
            Logger::logMessage(Logger::DEBUG, buffer);

            int lider = 0;
            bool hayLider = false;

            // Lo primero que hago es enviar mi id
            msgAlgoritmo.status = DESCONOCIDO;
            msgAlgoritmo.uid = idBroker;
            msgAlgoritmo.mtype = idGrupo;

            int idBrokerSiguiente = obtenerSiguiente(idBroker, idGrupo);
            if (idBrokerSiguiente == idBroker) {
                // No hay otros brokers en el anillo, soy el lider
                hayLider = true;
                lider = idBroker;
                iniciarMemoria(idGrupo);
            }
            else {
                enviarMensajeAlSiguiente(msgAlgoritmo,idBroker,idGrupo);
            }

            while (!hayLider) {
                char bufferMsgQueue[MSG_BROKER_SIZE];

                // Espero un mensaje del broker anterior
                colaLider.recv(idGrupo, bufferMsgQueue, MSG_BROKER_SIZE);
                memcpy(&msgAlgoritmo, bufferMsgQueue, sizeof (MsgAlgoritmoLider));

                if (msgAlgoritmo.status == DESCONOCIDO) {
                    if (msgAlgoritmo.uid == idBroker) {
                        // Me llego un mensaje con mi uid, por lo tanto soy el lider.
                        msgAlgoritmo.status = LIDER;
                        msgAlgoritmo.uid = idBroker;

                        // Envio un mensaje indicando que soy el lider
                        enviarMensajeAlSiguiente(msgAlgoritmo, idBroker, idGrupo);

                        iniciarMemoria(idGrupo);
                        lider = idBroker;
                        hayLider = true;

                    } else if (msgAlgoritmo.uid > idBroker) {
                        // Me llego un mensaje con un uid mayor, por lo tanto lo debo reenviar
                        enviarMensajeAlSiguiente(msgAlgoritmo, idBroker, idGrupo);
                    } else {
                        // Me llego un mensajes con un uid menor, por lo tanto se ignora el mensaje
                    }

                } else if (msgAlgoritmo.status == LIDER) {

                    // Marcar en una memoria compartida quien es el nuevo lider.
                    lider = msgAlgoritmo.uid;
                    idBrokerSiguiente = obtenerSiguiente(idBroker, idGrupo);
                    if (msgAlgoritmo.uid != idBrokerSiguiente) {
                        // Si el siguiente broker es el lider, no hace falta mandarle un mensaje
                        enviarMensajeAlSiguiente(msgAlgoritmo, idBroker, idGrupo);
                    }

                    hayLider = true;
                }
            }
            sprintf(buffer, "Se encontro lider para el grupo %d: %d", idGrupo, lider);
            Logger::logMessage(Logger::DEBUG, buffer);


            MsgEstadoBrokers mensajeEstadoBrokers;
            do
            {
                colaLider.recv(idGrupo, mensajeEstadoBrokers);

                TipoMensajeEstadoBroker estado = mensajeEstadoBrokers.estado;

                if(estado == REINICIAR_ALGORITMO)
                    continue;

                else if(estado == CAIDO)
                {
                    Logger::logMessage(Logger::DEBUG, "Recibe un mensaje de baja de broker.");
                    darBrokerDeBaja(mensajeEstadoBrokers.nroBroker);
                }

            } while(mensajeEstadoBrokers.estado != REINICIAR_ALGORITMO);

        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }

    return 0;
}


void darBrokerDeBaja(int nroBroker)
{
    /* Se deben recorrer todos los arreglos en los que se almacena
     * la relación entre tipos de agentes y broker en el que están;
     * de manera de dar de baja a todos los agentes conectados al
     * broker caído. */

    char buffer[1024];
    sprintf(buffer, "Se dará de baja al broker %d.", nroBroker);
    Logger::logMessage(Logger::DEBUG, buffer);

    IPC::SharedMemory<InformacionGrupoShMemBrokers> shMemInfoGruposShMemBrokers;
    shMemInfoGruposShMemBrokers.getSharedMemory(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS);

    IPC::Semaphore semInfoGruposShMemBrokers;
    semInfoGruposShMemBrokers.getSemaphore(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS, 1);

    InformacionGrupoShMemBrokers infoGrupoShMemBrokers;
    semInfoGruposShMemBrokers.wait();
    shMemInfoGruposShMemBrokers.read(&infoGrupoShMemBrokers);

    for(int idTipoAgente = 0; idTipoAgente < AMOUNT_AGENTS; idTipoAgente++)
    {
        IPC::SharedMemory<DataInfoAgentes> shMemInfoAgentes;
        shMemInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, idTipoAgente + 1);

        IPC::Semaphore semMutexShMemInfoAgentes;
        semMutexShMemInfoAgentes.getSemaphore(C_DIRECTORY_INFO_AGENTES, ID_INFO_AGENTES, AMOUNT_AGENTS);

        semMutexShMemInfoAgentes.wait(idTipoAgente);
        DataInfoAgentes dataInfoAgentes;
        shMemInfoAgentes.read(&dataInfoAgentes);

        for(int idAgente = 0; idAgente < MAX_AMOUNT_AGENTS; idAgente++)
        {
            if(dataInfoAgentes.agenteEnBroker[idAgente] == nroBroker)
            {
                /* Se encuentra un agente conectado al broker, se lo elimina. */
                sprintf(buffer, "Se detecta que un agente de tipo %d estaba conectado al broker %d.", idTipoAgente, nroBroker);
                Logger::logMessage(Logger::DEBUG, buffer);

                dataInfoAgentes.agenteEnBroker[idAgente] = 0;
                eliminarAgenteDeGrupo(&infoGrupoShMemBrokers, idTipoAgente);
            }
        }

        shMemInfoAgentes.write(&dataInfoAgentes);
        semMutexShMemInfoAgentes.signal(idTipoAgente);
    }

    shMemInfoGruposShMemBrokers.write(&infoGrupoShMemBrokers);
    semInfoGruposShMemBrokers.signal();
}

void eliminarAgenteDeGrupo(InformacionGrupoShMemBrokers* infoGrupoShMemBrokers, int idTipoAgente)
{
    /* Recorre todos los grupos de memoria compartida y si hay tipos de agentes registrados,
     * se elimina uno. */
    for(int nroGrupo = 0; nroGrupo < CANT_GRUPOS_SHMEM; nroGrupo++)
    {
        if(infoGrupoShMemBrokers->tiposDeAgenteRestantesPorGrupo[nroGrupo][idTipoAgente] > 0)
            infoGrupoShMemBrokers->tiposDeAgenteRestantesPorGrupo[nroGrupo][idTipoAgente]--;
    }
}

void enviarMensajeAlSiguiente(MsgAlgoritmoLider msg, int idBroker, int idGrupo) {
    // Obtengo la cola por la cual envio los mensajes al siguiente en el anillo
    IPC::MsgQueue colaBrokers = IPC::MsgQueue("Cola Bokers");
    colaBrokers.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);

    int idBrokerSiguiente = obtenerSiguiente(idBroker, idGrupo);
    if (idBrokerSiguiente != idBroker) {
        // Solo mando el mensaje si hay un siguiente a quien mandarlo
        MsgCanalEntradaBrokerBroker msgEntrada;
        memcpy(msgEntrada.msg, &msg, sizeof (MsgAlgoritmoLider));
        msgEntrada.tipoMensaje = MENSAJE_LIDER;

        MsgCanalSalidaBrokerBroker msgSalida;
        msgSalida.mtype = idBrokerSiguiente;
        memcpy(&msgSalida.msg, &msgEntrada, sizeof (MsgCanalEntradaBrokerBroker));
        colaBrokers.send(msgSalida);
    }
}

int obtenerSiguiente(int nroBroker, int nroGrupo)
{
    IPC::SharedMemory<InformacionGrupoShMemBrokers> shMemInfoGruposShMemBrokers;
    shMemInfoGruposShMemBrokers.getSharedMemory(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS);

    IPC::Semaphore semInfoGruposShMemBrokers;
    semInfoGruposShMemBrokers.getSemaphore(C_DIRECTORY_ADM, ID_IPC_INFO_GRUPOS_BROKERS, 1);

    InformacionGrupoShMemBrokers infoGrupoShMemBrokers;
    semInfoGruposShMemBrokers.wait();
    shMemInfoGruposShMemBrokers.read(&infoGrupoShMemBrokers);
    semInfoGruposShMemBrokers.signal();

    IPC::Semaphore semMutexDataInfoAgentes;
    semMutexDataInfoAgentes.getSemaphore(C_DIRECTORY_INFO_AGENTES, ID_INFO_AGENTES, AMOUNT_AGENTS);

    std::list<int> brokersEnElGrupo;
    for (int i = 0; i < AMOUNT_AGENTS; ++i) {
        if (infoGrupoShMemBrokers.tiposDeAgenteNecesariosPorGrupo[nroGrupo-ID_PRIMER_GRUPO_SHMEM][i] > 0) {
            // El agente pertence al grupo, debo verificar a que broker esta conectado
            IPC::SharedMemory<DataInfoAgentes> shMemDataInfoAgentes;
            shMemDataInfoAgentes.getSharedMemory(C_DIRECTORY_INFO_AGENTES, i + 1);

            DataInfoAgentes dataInfoAgentes;
            semMutexDataInfoAgentes.wait(i);
            shMemDataInfoAgentes.read(&dataInfoAgentes);
            semMutexDataInfoAgentes.signal(i);

            for(int nroAgenteEnBroker = 0; nroAgenteEnBroker < MAX_AMOUNT_AGENTS; nroAgenteEnBroker++) {
                if (dataInfoAgentes.agenteEnBroker[nroAgenteEnBroker] != 0) {
                    brokersEnElGrupo.push_front(dataInfoAgentes.agenteEnBroker[nroAgenteEnBroker]);
                }
            }
        }
    }

    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(SERVERS_CONFIG_FILE) );
    cfg->parse();

    int cantidadBrokers = cfg->getConfigFileParam("CantidadBrokers", -1);
    if ( cantidadBrokers == -1 ) {
        Logger::logMessage(Logger::ERROR, "Error al obtener cantidad de Brokers");
    }

    int siguiente = (nroBroker % cantidadBrokers) + 1;
    bool encontrado = false;

    while (!encontrado) {
        for (std::list<int>::iterator it = brokersEnElGrupo.begin(); it != brokersEnElGrupo.end(); ++it) {
            if ((*it) == siguiente) {
                encontrado = true;
            }
        }
        if (! encontrado) {
            siguiente = (siguiente % cantidadBrokers) + 1;
        }
    }

    /* Se imprime el mensaje cuando hay un cambio. */
    IPC::Semaphore semSiguienteBrokerAnillo("Siguiente Broker Sem");
    semSiguienteBrokerAnillo.getSemaphore(C_DIRECTORY_BROKER, ID_SIGUIENTE_BROKER, CANT_GRUPOS_SHMEM);

    IPC::SharedMemory<int> shMemSiguienteBrokerAnillo("Siguiente Broker ShMem");
    shMemSiguienteBrokerAnillo.getSharedMemory(C_DIRECTORY_BROKER, ID_SIGUIENTE_BROKER + nroGrupo);

    semSiguienteBrokerAnillo.wait(nroGrupo - ID_PRIMER_GRUPO_SHMEM);
    shMemSiguienteBrokerAnillo.write(&siguiente);
    semSiguienteBrokerAnillo.signal(nroGrupo - ID_PRIMER_GRUPO_SHMEM);

    return siguiente;
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

void iniciarMemoria(int idGrupo) {

    char buffer[MSG_BROKER_SIZE];
    MsgEntregaMemoriaAdministrador mensajeMemoria;
    mensajeMemoria.mtype = idGrupo;

    std::auto_ptr<IConfigFileParser> cfg(new ConfigFileParser(COMM_OBJECTS_CONFIG_FILE));
    cfg->parse();

    IPC::MsgQueue colaMemoria = IPC::MsgQueue("Cola Memoria");
    colaMemoria.getMsgQueue(C_DIRECTORY_BROKER, ID_TIPO_MEMORIA);

    if (idGrupo == cfg->getConfigFileParam("shMem-./DRobot5-4", -1)) {
        // Estado Robot5
        EstadoRobot5 estadoRobot5;
        estadoRobot5.robot5Bloqueado = false;
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &estadoRobot5, sizeof (EstadoRobot5));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DRobot11-2", -1)) {
        // Cinta 6-0
        CintaTransportadora_6 cinta6;
        cinta6.cantLibres = BUFF_SIZE_CINTA_6;
        cinta6.puntoLectura = 0;
        cinta6.robot11Durmiendo = false;
        for (int i = 0; i < BUFF_SIZE_CINTA_6; i++) cinta6.lugarVacio[i] = true;
        mensajeMemoria.mtype = idGrupo;
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &cinta6, sizeof (CintaTransportadora_6));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DRobot11-3", -1)) {
        // Cinta 6-1
        CintaTransportadora_6 cinta6;
        cinta6.cantLibres = BUFF_SIZE_CINTA_6;
        cinta6.puntoLectura = 0;
        cinta6.robot11Durmiendo = false;
        for (int i = 0; i < BUFF_SIZE_CINTA_6; i++) cinta6.lugarVacio[i] = true;
        mensajeMemoria.mtype = idGrupo;
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &cinta6, sizeof (CintaTransportadora_6));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DAGV-3", -1)) {
        // BufferCanastoEntre5yAGVSharedMemory 0 (No veo donde se inicializa...)
        Canasto canasto;
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &canasto, sizeof (Canasto));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DAGV-4", -1)) {
        // BufferCanastoEntre5yAGVSharedMemory 1 (No veo donde se inicializa...)
        Canasto canasto;

        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &canasto, sizeof (Canasto));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DAGV-5", -1)) {
        // BufferCanastoEntre5yAGVSharedMemory 2 (No veo donde se inicializa...)
        Canasto canasto;
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &canasto, sizeof (Canasto));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DAGV-9", -1)) {
        // BufferCanastos 0
        BufferCanastos canastos;
        for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++) {
            canastos.canastos[i].cantidadPiezas = 0;
            canastos.canastos[i].tipoPieza = NULL_PIEZA;
            canastos.robotCinta1EsperaPorElLugarNumero = -1;
            canastos.robotCinta2EsperaPorElLugarNumero = -1;
        }
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &canastos, sizeof (BufferCanastos));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DAGV-10", -1)) {
        // BufferCanastos 1
        BufferCanastos canastos;
        for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++) {
            canastos.canastos[i].cantidadPiezas = 0;
            canastos.canastos[i].tipoPieza = NULL_PIEZA;
            canastos.robotCinta1EsperaPorElLugarNumero = -1;
            canastos.robotCinta2EsperaPorElLugarNumero = -1;
        }
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &canastos, sizeof (BufferCanastos));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DAGV-11", -1)) {
        // BufferCanastos 2
        BufferCanastos canastos;
        for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++) {
            canastos.canastos[i].cantidadPiezas = 0;
            canastos.canastos[i].tipoPieza = NULL_PIEZA;
            canastos.robotCinta1EsperaPorElLugarNumero = -1;
            canastos.robotCinta2EsperaPorElLugarNumero = -1;
        }
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &canastos, sizeof (BufferCanastos));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DRobot11-6", -1)) {
        // SM-R11-R14
        DataSM_R11_R14 dataSM_R11_R14;
        SerializedData data = dataSM_R11_R14.serializeData();
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &data, sizeof (SerializedData));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DRobot14-2", -1)) {
        // SM-R14-R16
        DataSM_R14_R16 dataSM_R14_R16;
        SerializedData data = dataSM_R14_R16.serializeData();
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &data, sizeof (SerializedData));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DVendedor-1", -1)) {
        // AlmacenTerminados (No se lo inicializa en el Launcher, lo mismo se repite aqui)
        AlmacenProductosTerminados apt;

        for (int i = 0; i < TAM_ALMACEN; i++) {
            apt.almacen[i].esTemporal = false;
            apt.almacen[i].estado = VACIO;
            apt.almacen[i].idOrdenDeCompra = 0;
            apt.almacen[i].tipoProducto = 0;
        }
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &apt, sizeof (AlmacenProductosTerminados));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DVendedor-2", -1)) {
        // NumeroOrdenCompra
        int shMemData = 1;
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &shMemData, sizeof (int));

    } else if (idGrupo == cfg->getConfigFileParam("shMem-./DAPiezas-1", -1)) {
        // AlmacenDePiezas
        EstructuraAlmacenPiezas estructuraAlmacen;
        for (int i = 0; i < CANTIDAD_TIPOS_PIEZAS; ++i) estructuraAlmacen.cantCanastos[i] = 1;
        for (int i = 0; i < CANTIDAD_TIPOS_GABINETES; ++i) estructuraAlmacen.cantGabinetes[i] = 0;
        sprintf(buffer, "Se manda mensaje de inicio "
                "a administrador de SharedMemory %ld", mensajeMemoria.mtype);
        Logger::logMessage(Logger::IMPORTANT, buffer);
        memcpy(mensajeMemoria.memoria, &estructuraAlmacen, sizeof (EstructuraAlmacenPiezas));

    }

    memcpy(buffer, &mensajeMemoria, MSG_BROKER_SIZE);
    colaMemoria.send(buffer, MSG_BROKER_SIZE);
}

