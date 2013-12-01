#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include <Common.h>
#include <middlewareCommon.h>
#include <Exceptions/Exception.h>
#include "Logger/Logger.h"

#include <API/Objects/Util.h>
#include "IPCs/IPCTemplate/MsgQueue.h"

#include <Comunicaciones/Objects/CommMsgQueue.h>
#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/MiddlewareAPI.h>

void createIPCs();
void createDirectory(std::string directoryName);
void createMuxes();

int main(int argc, char* argv[]) {
    try {
        Util::getInstance();

        createDirectory(DIRECTORY_MUX);
        createDirectory(DIRECTORY_MUX);

        createIPCs();
        createMuxes();
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR,
        e.get_error_description().c_str());
        return -1;
    }

    return 0;
}

void createDirectory(std::string directoryName) {
    mkdir(directoryName.c_str(), 0777);
}

void createIPCs() {
    Logger::getInstance().createLog();
    Logger::getInstance().setProcessInformation("LauncherAgentes:");

    // Se crean las colas de cada tipo agente
    IPC::MsgQueue colaAgente;

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_CLIENTE);
    Logger::logMessage(Logger::COMM, "Cola Agente-cliente creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_VENDEDOR);
    Logger::logMessage(Logger::COMM, "Cola Agente-vendedor creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_AP);
    Logger::logMessage(Logger::COMM, "Cola Agente-AlmacenDePiezas creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_AGV);
    Logger::logMessage(Logger::COMM, "Cola Agente-AGV creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_ROBOT5_AGV);
    Logger::logMessage(Logger::COMM, "Cola Agente-Robot5AGV creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_ROBOT5_CINTA);
    Logger::logMessage(Logger::COMM, "Cola Agente-Robot5Cinta creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_ROBOT11);
    Logger::logMessage(Logger::COMM, "Cola Agente-Robot11 creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_ROBOT12);
    Logger::logMessage(Logger::COMM, "Cola Agente-Robot12 creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_ROBOT14);
    Logger::logMessage(Logger::COMM, "Cola Agente-Robot14 creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_ROBOT16_CINTA);
    Logger::logMessage(Logger::COMM, "Cola Agente-Robot16Cinta creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_ROBOT16_DESPACHO);
    Logger::logMessage(Logger::COMM, "Cola Agente-Robot16Despacho creada");

    colaAgente.create(DIRECTORY_MUX, ID_TIPO_DESPACHO);
    Logger::logMessage(Logger::COMM, "Cola Agente-Despacho creada");


    IPC::CommMsgQueue commColaSalida;

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_CLIENTE);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-cliente creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_VENDEDOR);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-vendedor creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_AP);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-AlmacenDePiezas creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_AGV);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-AGV creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_ROBOT5_AGV);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot5AGV creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_ROBOT5_CINTA);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot5Cinta creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_ROBOT11);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot11 creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_ROBOT12);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot12 creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_ROBOT14);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot14 creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_ROBOT16_CINTA);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot16Cinta creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_ROBOT16_DESPACHO);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Robot16Despacho creada");

    commColaSalida.create(DIRECTORY_COMM, ID_TIPO_DESPACHO);
    Logger::logMessage(Logger::COMM, "Cola CanalSalida-Despacho creada");
}

void createMuxes() {
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_CLIENTE));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_VENDEDOR));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_AP));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_AGV));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_ROBOT5_CINTA));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_ROBOT5_AGV));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_ROBOT11));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_ROBOT12));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_ROBOT14));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_ROBOT16_CINTA));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_ROBOT16_DESPACHO));
    Util::createProcess("MuxTipoAgente", static_cast<int>(ID_TIPO_DESPACHO));
}
