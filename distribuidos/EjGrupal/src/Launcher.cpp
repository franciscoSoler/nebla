/* 
 * File:   Launcher.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:37
 */
#include <iostream>

#include <sys/stat.h>
#include <sys/types.h>

#include "Common.h"
#include "Exceptions/Exception.h"
#include "Logger/Logger.h"

#include "IPCs/Semaphore/Semaphore.h"

#include "IPCs/IPCAbstractos/MessageQueue/Barrera1112MessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosAgvMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/ComunicacionRobot5MessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"

#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.h"
#include "IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"

static char buffer[255];
static char param1[20];
static char param2[20];

void createIPCs();
void createDirectory(std::string directoryName);
void createProcess(std::string processName, int amountOfProcesses = 1);

int main(int argc, char* argv[]) {
    try {

        createDirectory(DIRECTORY_AGV);
        createDirectory(DIRECTORY_ROBOT_5);
        createDirectory(DIRECTORY_ROBOT_11);
        createDirectory(DIRECTORY_ROBOT_12);
        
        createIPCs();
        
        //createProcess("Robot5", 1);
        //createProcess("ControladorRobot5Agv", 1);
        //createProcess("ControladorRobot5Cinta", 1);
                                
        //createProcess("Robot11", 2);
        //createProcess("Robot12", 2);
        //createProcess("AGV", 3);
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
    Logger::getInstance().setProcessInformation("Launcher:");
    
    // Robot 5 - Cinta
    IPC::PedidosProduccionMessageQueue colaPedidos("PedidosProduccionMessageQueue");
    colaPedidos.createMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_PEDIDOS_PRODUCCION);
            
    IPC::Semaphore semaforoAccesoEstadoRobot5("Estado Robot 5");
    semaforoAccesoEstadoRobot5.createSemaphore(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);
    semaforoAccesoEstadoRobot5.initializeSemaphore(0,1);
    IPC::EstadoRobot5SharedMemory estadoRobot5ShMem("EstadoRobot5SharedMemory");
    estadoRobot5ShMem.createSharedMemory(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);
    semaforoAccesoEstadoRobot5.wait();
    {
        EstadoRobot5 estadoRobot5;
        estadoRobot5.robot5Bloqueado = false;
        estadoRobot5ShMem.writeInfo(&estadoRobot5);
    }
    semaforoAccesoEstadoRobot5.signal();
    
    IPC::Semaphore semaforoBloqueoRobot5("Bloqueo Robot 5");
    semaforoBloqueoRobot5.createSemaphore(DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);
    semaforoBloqueoRobot5.initializeSemaphore(0,0);
    
    IPC::Semaphore semaforoBloqueoRobot11("Bloqueo Robot 11");
    semaforoBloqueoRobot11.createSemaphore(DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, CANTIDAD_CINTAS_6);
    for (int i = 0; i < CANTIDAD_CINTAS_6; ++i) {
        semaforoBloqueoRobot11.initializeSemaphore(i, 0);
    }
    
    IPC::Semaphore semaforoAccesoCinta6("Acceso Cinta 6");
    semaforoAccesoCinta6.createSemaphore(DIRECTORY_ROBOT_11, ID_SEM_CINTA_6, CANTIDAD_CINTAS_6);
        for (int i = 0; i < CANTIDAD_CINTAS_6; ++i) {
        semaforoAccesoCinta6.initializeSemaphore(i, 1);
    }
    
    IPC::Cinta6SharedMemory cinta6_0("Cinta 6 Share dMemory 0");
    cinta6_0.createSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_0);
    IPC::Cinta6SharedMemory cinta6_1("Cinta 6 Share dMemory 1");
    cinta6_1.createSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_1);

    CintaTransportadora_6 cinta6;
    cinta6.cantLibres = BUFF_SIZE_CINTA_6;
    cinta6.puntoLectura = 0;
    cinta6.robot11Durmiendo = false;
    for (int i = 0; i < BUFF_SIZE_CINTA_6; i++) cinta6.lugarVacio[i] = true;

    semaforoAccesoCinta6.wait(0);
    {
        cinta6_0.writeInfo(&cinta6);
    }
    semaforoAccesoCinta6.signal(0);    
    
    semaforoAccesoCinta6.wait(1);
    {
        cinta6_1.writeInfo(&cinta6);
    }
    semaforoAccesoCinta6.signal(1);
    
    //Robot 5 - AGV
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV1("BufferCanastoEntre5yAGVSharedMemory 0");
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV2("BufferCanastoEntre5yAGVSharedMemory 1");
    IPC::BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV3("BufferCanastoEntre5yAGVSharedMemory 2");
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
    IPC::PedidosAgvMessageQueue colaPedidosAGV_5;
    colaPedidosAGV_5.createMessageQueue(DIRECTORY_AGV, ID_COLA_PEDIDOS_AGV_5);
    
    //exclusivo Robot 5
    /* Creo la cola de comunicacion con el robot 5 con los dos procesos del controlador */
    IPC::ComunicacionRobot5MessageQueue colaComunicacionRobot5 = IPC::ComunicacionRobot5MessageQueue("colaComunicacionRobot5");
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

