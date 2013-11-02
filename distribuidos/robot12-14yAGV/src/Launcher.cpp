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
    
    BufferCanastos canastos;
    for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++) {
        canastos.canastos[i].cantidadPiezas = 0;
        canastos.canastos[i].tipoPieza = PIEZA_1;
    }
    
    canastos.robotCinta1EsperaPorElLugarNumero = -1;
    canastos.robotCinta2EsperaPorElLugarNumero = -1;
    CintaTransportadora_6 cinta6;
    cinta6.cantLibres = BUFF_SIZE_CINTA_6;
    cinta6.puntoLectura = 0;
    cinta6.puntoEscritura = BUFF_SIZE_CINTA_6 - 1;
    cinta6.robot11Durmiendo = false;
    for (int i = 0; i < BUFF_SIZE_CINTA_6; i++)
        cinta6.lugarVacio[i] = true;
    
    // Create and initialize ShMem
    BufferCanastoSharedMemory shMemCanastos1;
    BufferCanastoSharedMemory shMemCanastos2;
    BufferCanastoSharedMemory shMemCanastos3;
    shMemCanastos1.createSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_1);
    shMemCanastos2.createSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_2);
    shMemCanastos3.createSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_3);
    shMemCanastos1.writeInfo(&canastos);
    shMemCanastos2.writeInfo(&canastos);
    shMemCanastos3.writeInfo(&canastos);
    
    //Logger::logMessage(Logger::IMPORTANT, "IPC shMem_bufferCanastos1, 2, 3 creados");
    
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV1;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV2;
    BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV3;
    shMemPasajeCanastoEntre5yAGV1.createSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_1);
    shMemPasajeCanastoEntre5yAGV2.createSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_2);
    shMemPasajeCanastoEntre5yAGV3.createSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_3);
    
    //Logger::logMessage(Logger::IMPORTANT, "IPC shMemPasajeCanastoEntre5yAGV1, 2, 3 creados");
    
    EstadoRobot5SharedMemory shMemEstadoRobot5;
    shMemEstadoRobot5.createSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_5);
    
    //Logger::logMessage(Logger::IMPORTANT, "IPC shMemEstadoRobot5 creado");
            
    Cinta6SharedMemory shMemCinta61;
    Cinta6SharedMemory shMemCinta62;
    shMemCinta61.createSharedMemory(DIRECTORY, ID_MEM_CINTA_6_1);
    shMemCinta62.createSharedMemory(DIRECTORY, ID_MEM_CINTA_6_2);
    shMemCinta61.writeInfo(&cinta6);
    shMemCinta62.writeInfo(&cinta6);
   
    //Logger::logMessage(Logger::IMPORTANT, "IPC shMemCinta61, 2 creados");
    
    // Do the same with the semaphores
    IPC::Semaphore semEstAGV("semEstadoAGV");
    IPC::Semaphore semMemAGV_5("semMemAGV_5");
    IPC::Semaphore semMemCanastos("semMemCanastos");
    semEstAGV.createSemaphore(DIRECTORY, ID_SEM_EST_AGV, CANTIDAD_AGVS);
    semMemAGV_5.createSemaphore(DIRECTORY, ID_SEM_MEM_AGV_5, CANTIDAD_AGVS);
    semMemCanastos.createSemaphore(DIRECTORY, ID_SEM_MEM_CANASTOS_10, CANTIDAD_AGVS);
    for (int i = 0; i < CANTIDAD_AGVS; i++) {
        semEstAGV.initializeSemaphore(i, 0);
        semMemAGV_5.initializeSemaphore(i, 1);
        semMemCanastos.initializeSemaphore(i, 1);
    }
    
    IPC::Semaphore semEstadoRobot5("semEstadoRobot5");
    semEstadoRobot5.createSemaphore(DIRECTORY, ID_SEM_EST_ROBOT_5, CANTIDAD_AGVS);
    semEstadoRobot5.initializeSemaphore(0, 0);
    
    IPC::Semaphore semMemEstadoRobot5("semMemEstadoRobot5");
    semMemEstadoRobot5.createSemaphore(DIRECTORY, ID_SEM_MEM_EST_ROBOT_5, 1);
    semMemEstadoRobot5.initializeSemaphore(0, 1);
    
    IPC::Semaphore semMemCinta6("semMemCinta6");
    IPC::Semaphore semRobot11("semRobot11");
    IPC::Semaphore semRobot12("semRobot12");
    semMemCinta6.createSemaphore(DIRECTORY, ID_SEM_MEM_CINTA_6, CANTIDAD_CINTAS_6);
    semRobot11.createSemaphore(DIRECTORY, ID_SEM_ROBOT_11, CANTIDAD_CINTAS_6);
    semRobot12.createSemaphore(DIRECTORY, ID_SEM_ROBOT_12, CANTIDAD_CINTAS_6);
    for (int i = 0; i < CANTIDAD_CINTAS_6; i++)  {
        semMemCinta6.initializeSemaphore(i, 1);
        semRobot11.initializeSemaphore(i, 0);
        semRobot12.initializeSemaphore(i, 0);
    }   
    
    // Do the same with the queues
    PedidosAGVMessageQueue colaPedidosAGV;
    colaPedidosAGV.create(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
    
    PedidosCanastosMessageQueue colaPedidosCanastos;
    colaPedidosCanastos.create(DIRECTORY,ID_COLA_PEDIDOS_A_AGV);
    
    Barrera1112MessageQueue cola11121;
    Barrera1112MessageQueue cola11122;
    cola11121.create(DIRECTORY,ID_COLA_11_12_1);
    cola11122.create(DIRECTORY,ID_COLA_11_12_2);
    
    Barrera1112MessageQueue cola12111;
    Barrera1112MessageQueue cola12112;
    cola12111.create(DIRECTORY,ID_COLA_12_11_1);
    cola12112.create(DIRECTORY,ID_COLA_12_11_2);
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

