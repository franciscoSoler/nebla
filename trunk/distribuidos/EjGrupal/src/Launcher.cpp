/* 
 * File:   Launcher.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:37
 */
#include <iostream>
#include "Common.h"
#include "Exceptions/Exception.h"
#include "Logger/Logger.h"
#include "IPC/Semaphore/Semaphore.h"
#include "IPC/MessageQueue/Barrera1112MessageQueue.h"
#include "IPC/MessageQueue/PedidosAGVMessageQueue.h"
#include "IPC/MessageQueue/PedidosCanastosMessageQueue.h"
#include "IPC/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "IPC/SharedMemory/BufferCanastoSharedMemory.h"
#include "IPC/SharedMemory/Cinta6SharedMemory.h"
#include "IPC/SharedMemory/EstadoRobot5SharedMemory.h"

static char buffer[255];
static char param1[20];
static char param2[20];

void createIPCs();
void createProcess(std::string processName, int amountOfProcesses = 1);

int main(int argc, char* argv[]) {
    try {
        createIPCs();
        createProcess("Robot11", 2);
        createProcess("Robot12", 2);
        createProcess("AGV", 3);
        createProcess("MockRobot5AGV");
        createProcess("MockRobot5Cinta");
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, 
        e.get_error_description().c_str());
        return -1;
    }

    return 0;
}


void createIPCs() {
    Logger::getInstance().createLog();
    Logger::getInstance().setProcessInformation("Launcher:");
    
    //Robot 5 - AGV
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV1;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV2;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV3;
    shMemPasajeCanastoEntre5yAGV1.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_0);
    shMemPasajeCanastoEntre5yAGV2.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_1);
    shMemPasajeCanastoEntre5yAGV3.createSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_2);
    
    // Do the same with the semaphores
    IPC::Semaphore semBloqueoAGV("semBloqueoAGV");
    IPC::Semaphore semBufferAGV_5("semBufferAGV_5");
    semBloqueoAGV.createSemaphore(DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, CANTIDAD_AGVS);
    semBufferAGV_5.createSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, CANTIDAD_AGVS);
    
    for (int i = 0; i < CANTIDAD_AGVS; i++) {
        semBloqueoAGV.initializeSemaphore(i, 0);
        semBufferAGV_5.initializeSemaphore(i, 1);
    }
    
    // Do the same with the queues
    PedidosAGVMessageQueue colaPedidosAGV_5;
    colaPedidosAGV_5.create(DIRECTORY_AGV, ID_COLA_PEDIDOS_AGV_5);
    
    //exclusivo Robot 5
    /* Creo la cola de comunicacion con el robot 5 con los dos procesos del controlador */
    ComunicacionRobot5MessageQueue colaComunicacionRobot5 = ComunicacionRobot5MessageQueue();
    colaComunicacionRobot5.createMessageQueue(DIRECTORY_ROBOT_5, ID_COLA_API_ROBOT_5);
    
}

void createProcess(std::string processName, int amountOfProcesses) {
	pid_t pid;

    for (int i = 0; i < amountOfProcesses; i++) {
        if ((pid = fork()) < 0) {
            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);
        }
        else if (pid == 0) {
            // Child process. Pass the arguments to the process and call exec
            sprintf(param1, "%d", i);
            sprintf(buffer, "./%s", processName.c_str());
            execlp(buffer, processName.c_str(), param1, (char *) 0);

            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);

            return;
        }
    }
}

