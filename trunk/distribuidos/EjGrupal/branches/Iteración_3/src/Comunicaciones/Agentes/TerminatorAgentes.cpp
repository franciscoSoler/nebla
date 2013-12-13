#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include <Exceptions/Exception.h>
#include "Logger/Logger.h"

#include <API/Objects/Util.h>
#include "IPCs/IPCTemplate/MsgQueue.h"

#include <Comunicaciones/Objects/ServersManager.h>
#include <middlewareCommon.h>

void getMsgQueueIPCs();
void getMsgQueueDirectory(std::string directoryName);

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("TerminatorAgentes:");

        // Colas para el envío de mensajes
        IPC::MsgQueue commColaSalida;

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




        // Colas para Semáforos y SharedMemories
        IPC::MsgQueue commSemaphore;

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_CLIENTE);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-cliente destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_VENDEDOR);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-vendedor destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_AP);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-AlmacenDePiezas destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_AGV);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-AGV destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_ROBOT5_AGV);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Robot5AGV destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_ROBOT5_CINTA);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Robot5Cinta destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_ROBOT11);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Robot11 destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_ROBOT12);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Robot12 destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_ROBOT14);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Robot14 destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_ROBOT16_CINTA);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Robot16Cinta destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_ROBOT16_DESPACHO);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Robot16Despacho destruida");

        commSemaphore.getMsgQueue(DIRECTORY_SEM, ID_TIPO_DESPACHO);
        commSemaphore.destroy();
        Logger::logMessage(Logger::COMM, "Cola Canal-Semaphore-SharedMemory-Despacho destruida");

    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
    }

    return 0;
}

