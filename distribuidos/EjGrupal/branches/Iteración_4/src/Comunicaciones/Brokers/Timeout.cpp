#include <memory>

#include "../../Common.h"
#include "../../middlewareCommon.h"

#include "../Objects/ArgumentParser.h"

#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCTemplate/MsgQueue.h"
#include "../../IPCs/IPCTemplate/SharedMemory.h"

#include "../Objects/CommPacketWrapper.h"

#include <ConfigFileParser/ConfigFileParser.h>

static const char* C_DIRECTORY_BROKER = NULL;

void seleccionarDirectorio(int brokerNumber);

int main(int argc, char* argv[])
{
    Logger::setProcessInformation("Timeout:");

    char buffer[TAM_BUFFER];
    ArgumentParser argParser(argc, argv);

    int idBroker;
    if(argParser.parseArgument(1, idBroker) == -1)
    {
        Logger::logMessage(Logger::ERROR, "ERROR: parseArgument 1.");
        exit(-1);
    }

    int idBrokerRemoto;
    if(argParser.parseArgument(2, idBrokerRemoto) == -1)
    {
        Logger::logMessage(Logger::ERROR, "ERROR: parseArgument 2.");
        exit(-1);
    }

    ulong idMensajeTimeout;
    if(argParser.parseArgument(3, idMensajeTimeout) == -1)
    {
        Logger::logMessage(Logger::ERROR, "ERROR: parseArgument 3.");
        exit(-1);
    }

    std::auto_ptr<IConfigFileParser> cfg( new ConfigFileParser(SERVERS_CONFIG_FILE) );
    cfg->parse();

    int cantidadBrokers = cfg->getConfigFileParam("CantidadBrokers", -1);
    if (cantidadBrokers == -1)
        Logger::logMessage(Logger::ERROR, "Error al obtener cantidad de Brokers");

    sprintf(buffer, "Timeout B%d:", idBroker);
    Logger::setProcessInformation(buffer);

    seleccionarDirectorio(idBroker);

    IPC::Semaphore semUltimoACKRecibido;
    semUltimoACKRecibido.getSemaphore(C_DIRECTORY_BROKER, ID_SEM_TIMEOUT, cantidadBrokers);

    sprintf(buffer, "Obtengo sem Timeout con directorio %s, id %d y cantSemáforos %d (IPC TIMEOUT).", C_DIRECTORY_BROKER, ID_SEM_TIMEOUT, cantidadBrokers);
    Logger::logMessage(Logger::DEBUG, buffer);

    sprintf(buffer, "Va a tratar de obtener la memoria compartida con directorio %s y ID %d (IPC TIMEOUT).", C_DIRECTORY_BROKER, ID_SHMEM_TIMEOUT + idBrokerRemoto - 1);
    Logger::logMessage(Logger::DEBUG, buffer);

    IPC::SharedMemory<ulong> shMemUltimoACKRecibido;
    shMemUltimoACKRecibido.getSharedMemory(C_DIRECTORY_BROKER, ID_SHMEM_TIMEOUT + idBrokerRemoto - 1);

    sleep(TIEMPO_TIMEOUT);

    try
    {
        ulong idUltimoMensajeRecibido;

        sprintf(buffer, "Quiero entrar a la shMem haciendo p() en el sem %d (IPC TIMEOUT).", idBrokerRemoto - 1);
        Logger::logMessage(Logger::DEBUG, buffer);

        semUltimoACKRecibido.wait(idBrokerRemoto - 1);
        shMemUltimoACKRecibido.read(&idUltimoMensajeRecibido);
        semUltimoACKRecibido.signal(idBrokerRemoto - 1);

        if(idMensajeTimeout > idUltimoMensajeRecibido)
        {
            sprintf(buffer, "TIMEOUT ERROR: esperaba algo mayor o igual a %lu; el último mensaje recibido fue %lu.", idMensajeTimeout, idUltimoMensajeRecibido);
            Logger::logMessage(Logger::ERROR, buffer);

            /* Envía mensaje de error al pobre canal salida broker-broker. */
            MsgCanalEntradaBrokerBroker msgACKEntrada;
            MsgCanalSalidaBrokerBroker msgACKSalida;

            msgACKEntrada.tipoMensaje = MENSAJE_TIMEOUT;
            msgACKEntrada.msg_id = idMensajeTimeout;
            msgACKSalida.mtype = idBrokerRemoto;
            memcpy(&msgACKSalida.msg, &msgACKEntrada, sizeof(MsgCanalEntradaBrokerBroker));

            IPC::MsgQueue colaCanalSalidaBrokerBroker;
            colaCanalSalidaBrokerBroker.getMsgQueue(C_DIRECTORY_BROKER, ID_MSG_QUEUE_CSBB);
            colaCanalSalidaBrokerBroker.send(msgACKSalida);

            sprintf(buffer, "Envío mensaje de Broker caído (ACK %lu) al broker B%d!", idMensajeTimeout, idBroker);
            Logger::logMessage(Logger::DEBUG, buffer);

            return -1;
        }

        else
        {
            sprintf(buffer, "TIMEOUT con id %lu lee que el último ACK recibido es %lu y se muere feliz.", idMensajeTimeout, idUltimoMensajeRecibido);
            Logger::logMessage(Logger::COMM, buffer);
        }

    }

    catch(Exception & ex)
    {
        Logger::logMessage(Logger::ERROR, ex.get_error_description());
        abort();
    }

    return 0;
}

void seleccionarDirectorio(int idBroker) {
    char buffer[1024];
    sprintf(buffer, "Voy a seleccionar directorio con mi id: %d (TIMEOUT)", idBroker);
    Logger::logMessage(Logger::DEBUG, buffer);

    switch (idBroker) {
        case 1:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_1;
            break;
        case 2:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_2;
            break;
        case 3:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_3;
            break;
        case 4:
            C_DIRECTORY_BROKER = DIRECTORY_BROKER_4;
            break;
        default:
            Logger::logMessage(Logger::ERROR, "Error al elegir directorio del Broker.");
            abort();
    }
}

