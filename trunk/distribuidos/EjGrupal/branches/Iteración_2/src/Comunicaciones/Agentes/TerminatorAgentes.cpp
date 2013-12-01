#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include <Exceptions/Exception.h>
#include "Logger/Logger.h"

#include <API/Objects/Util.h>
#include "IPCs/IPCTemplate/MsgQueue.h"
#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/CommMsgQueue.h>

void getMsgQueueIPCs();
void getMsgQueueDirectory(std::string directoryName);

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("TerminatorAgentes:");

        // Se crea una cola por cada Agente
        IPC::MsgQueue colaAgente;

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_CLIENTE);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-cliente destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_VENDEDOR);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-vendedor destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_AP);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-AlmacenDePiezas destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_AGV);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-AGV destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_ROBOT5_AGV);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Robot5AGV destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_ROBOT5_CINTA);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Robot5Cinta destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_ROBOT11);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Robot11 destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_ROBOT12);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Robot12 destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_ROBOT14);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Robot14 destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_ROBOT16_CINTA);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Robot16Cinta destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_ROBOT16_DESPACHO);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Robot16Despacho destruida");

        colaAgente.getMsgQueue(DIRECTORY_MUX, ID_TIPO_DESPACHO);
        colaAgente.destroy();
        Logger::logMessage(Logger::COMM, "Cola Agente-Despacho destruida");


        IPC::CommMsgQueue commColaSalida;

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_CLIENTE);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-cliente destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_VENDEDOR);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-vendedor destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_AP);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-AlmacenDePiezas destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_AGV);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-AGV destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_ROBOT5_AGV);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot5AGV destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_ROBOT5_CINTA);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot5Cinta destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_ROBOT11);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot11 destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_ROBOT12);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot12 destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_ROBOT14);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot14 destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_ROBOT16_CINTA);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot16Cinta destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_ROBOT16_DESPACHO);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot16Despacho destruida");

        commColaSalida.getMsgQueue(DIRECTORY_COMM, ID_TIPO_DESPACHO);
        commColaSalida.destroy();
        Logger::logMessage(Logger::COMM, "Cola CanalSalida-Despacho destruida");

    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }

    return 0;
}

