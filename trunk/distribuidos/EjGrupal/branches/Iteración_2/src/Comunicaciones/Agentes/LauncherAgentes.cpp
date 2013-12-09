#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include <Common.h>
#include <middlewareCommon.h>
#include <Exceptions/Exception.h>
#include "Logger/Logger.h"

#include <API/Objects/Util.h>
#include "IPCs/IPCTemplate/MsgQueue.h"

#include <Comunicaciones/Objects/ServersManager.h>
#include <Comunicaciones/Objects/MiddlewareAPI.h>

void createIPCs();
void createDirectory(std::string directoryName);

int main(int argc, char* argv[]) {
    try {
        Util::getInstance();
        createDirectory(DIRECTORY_COMM);
        createDirectory(DIRECTORY_SEM);
        createIPCs();
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

    IPC::MsgQueue commColaSalida;

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
    
    commColaSalida.create(DIRECTORY_SEM, ID_COMM_SEM_ENTRADA);
    Logger::logMessage(Logger::COMM, "Cola CanalEntrada-Semaforos creada");
}
