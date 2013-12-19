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

#include "../../Common.h"
#include "../../middlewareCommon.h"

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

int enviarMensajeAlSiguiente(MsgAlgoritmoLider msg, int idGrupo, int idBroker);
int obtenerSiguiente(int idGrupo, int idBroker);
void elegirDirectorios(int brokerNumber);
void iniciarMemoria(int idGrupo);

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

    sprintf(buffer, "Algoritmo Lider Nº%d:", idGrupo);
    Logger::setProcessInformation(buffer);

    Logger::logMessage(Logger::DEBUG, "Algoritmo del lider iniciado satisfactoriamente");
   
    elegirDirectorios( idBroker );

    // Obtengo la cola por la cual recibo los mensajes del algoritmo
    IPC::MsgQueue colaLider = IPC::MsgQueue("Cola Lider");
    colaLider.getMsgQueue(C_DIRECTORY_BROKER, ID_ALGORITMO_LIDER);

    while (true) {
        // Me bloqueo esperando que deba iniciar el algoritmo
        MsgAlgoritmoLider msgAlgoritmo;

        char bufferMsgQueue[MSG_BROKER_SIZE];
        colaLider.recv(idGrupo, bufferMsgQueue, MSG_BROKER_SIZE);
        memcpy(&msgAlgoritmo, bufferMsgQueue, sizeof (MsgAlgoritmoLider));

        if (msgAlgoritmo.status != INICIAR) {
            // Para desbloquear el algoritmo del lider el primer mensage debe ser de INICIAR
            Logger::logMessage(Logger::ERROR, "Se esta intentando iniciar el algoritmo del lider con un mensaqe invalido.");
            abort();
        }

        bool hayLider = false;

        // Lo primero que hago es enviar mi id
        msgAlgoritmo.status = DESCONOCIDO;
        msgAlgoritmo.uid = idBroker;
        msgAlgoritmo.mtype = idGrupo;
        
        if (enviarMensajeAlSiguiente(msgAlgoritmo, idGrupo, idBroker) == idBroker) {
            // No hay otros brokers en el anillo, soy el lider
            hayLider = true;
            iniciarMemoria(idGrupo);
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
                    enviarMensajeAlSiguiente(msgAlgoritmo, idGrupo, idBroker);
                    
                    sprintf(buffer, "SOY EL LIDER: %d:", idBroker);
                    Logger::logMessage(Logger::COMM, buffer);
                    
                    iniciarMemoria(idGrupo);
                    hayLider = true;

                } else if (msgAlgoritmo.uid > idBroker) {
                    // Me llego un mensaje con un uid mayor, por lo tanto lo debo reenviar
                    enviarMensajeAlSiguiente(msgAlgoritmo, idGrupo, idBroker);
                } else {
                    // Me llego un mensajes con un uid menor, por lo tanto se ignora el mensaje
                }
            } else if (msgAlgoritmo.status == LIDER) {
                
                // Marcar en una memoria compartida quien es el nuevo lider.
                sprintf(buffer, "SE ENCONTRO QUE EL LIDER ES: %d:", idBroker);
                Logger::logMessage(Logger::COMM, buffer);
                
                int siguiente = obtenerSiguiente(idGrupo, idBroker);
                if (msgAlgoritmo.uid != siguiente) {
                    // Si el siguiente broker es el lider, no hace falta mandarle un mensaje                        
                    enviarMensajeAlSiguiente(msgAlgoritmo, idGrupo, idBroker);
                }
                
                hayLider = true;
            }
        }
    }

    return 0;
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

int enviarMensajeAlSiguiente(MsgAlgoritmoLider msg, int idGrupo, int idBroker) {
    
    // Obtengo la cola por la cual envio los mensajes al siguiente en el anillo
    IPC::MsgQueue colaBrokers = IPC::MsgQueue("Cola Bokers");
    colaBrokers.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);
    
    int siguiente = obtenerSiguiente(idGrupo, idBroker);

    if (siguiente != idBroker) {
        // Solo mando el mensaje si hay un siguiente a quien mandarlo
        MsgCanalEntradaBrokerBroker msgEntrada;
        memcpy(msgEntrada.msg, &msg, sizeof (MsgAlgoritmoLider));
        msgEntrada.tipoMensaje = MENSAJE_LIDER;

        MsgCanalSalidaBrokerBroker msgSalida;
        msgSalida.mtype = siguiente;
        memcpy(&msgSalida.msg, &msgEntrada, sizeof (MsgCanalEntradaBrokerBroker));
        colaBrokers.send(msgSalida);
    }

    return siguiente;
}

int obtenerSiguiente(int idGrupo, int idBroker) {

    // Obtengo la memoria compartida con el siguiente broker
    IPC::SharedMemory<int> siguienteSharedMemory("Siguiente Broker ShMem");
    siguienteSharedMemory.getSharedMemory(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE);
    Logger::logMessage(Logger::COMM, "shMem SiguienteBroker creado");

    IPC::Semaphore semaforoSiguiente = IPC::Semaphore("Semaforo Siguiente Broker");
    semaforoSiguiente.getSemaphore(C_DIRECTORY_BROKER, ID_SHMEM_SIGUIENTE, 1);
    Logger::logMessage(Logger::COMM, "Semaforo shMem SiguienteBroker creado");            
    
    int siguiente;
    semaforoSiguiente.wait();
    siguienteSharedMemory.read(&siguiente);
    semaforoSiguiente.signal();
    return siguiente;
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
