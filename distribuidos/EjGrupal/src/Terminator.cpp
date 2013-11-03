/* 
 * File:   Terminator.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:37
 */
#include <iostream>
#include "Common.h"
#include "Exceptions/Exception.h"
#include "Logger/Logger.h"
#include "IPCs/Semaphore/Semaphore.h"
#include "IPCs/MessageQueue/Barrera1112MessageQueue.h"
#include "IPCs/MessageQueue/PedidosAGVMessageQueue.h"
#include "IPCs/MessageQueue/PedidosCanastosMessageQueue.h"
#include "IPCs/SharedMemory/BufferCanastoEntre5yAGVSharedMemory.h"
#include "IPCs/SharedMemory/BufferCanastoSharedMemory.h"
#include "IPCs/SharedMemory/Cinta6SharedMemory.h"
#include "IPCs/SharedMemory/EstadoRobot5SharedMemory.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("Terminator:");
        
        BufferCanastoSharedMemory shMemCanastos1;
        BufferCanastoSharedMemory shMemCanastos2;
        BufferCanastoSharedMemory shMemCanastos3;
        shMemCanastos1.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_1);
        shMemCanastos2.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_2);
        shMemCanastos3.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_3);
        shMemCanastos1.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_bufferCanastos1 destruido");
        shMemCanastos2.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_bufferCanastos2 destruido");
        shMemCanastos3.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_bufferCanastos3 destruido");
        
        BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV1;
        BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV2;
        BufferCanastoEntre5yAGVSharedMemory shMemPasajeCanastoEntre5yAGV3;
        shMemPasajeCanastoEntre5yAGV1.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_1);
        shMemPasajeCanastoEntre5yAGV2.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_2);
        shMemPasajeCanastoEntre5yAGV3.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_3);
        shMemPasajeCanastoEntre5yAGV1.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_bufferCanastoEntreAGVy51 destruido");
        shMemPasajeCanastoEntre5yAGV2.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_bufferCanastoEntreAGVy52 destruido");
        shMemPasajeCanastoEntre5yAGV3.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_bufferCanastoEntreAGVy53 destruido");

        EstadoRobot5SharedMemory shMemEstadoRobot5;
        shMemEstadoRobot5.getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_5);
        shMemEstadoRobot5.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_estado_robot_5 destruido");

        Cinta6SharedMemory shMemCinta61;
        Cinta6SharedMemory shMemCinta62;
        shMemCinta61.getSharedMemory(DIRECTORY, ID_MEM_CINTA_6_1);
        shMemCinta62.getSharedMemory(DIRECTORY, ID_MEM_CINTA_6_2);
        shMemCinta61.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_cinta_6_1 destruido");
        shMemCinta62.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_cinta_6_2 destruido");
        
        //semaphores
        IPC::Semaphore semEstAGV("semEstadoAGV");
        IPC::Semaphore semMemAGV_5("semMemAGV_5");
        IPC::Semaphore semMemCanastos("semMemCanastos");
        semEstAGV.getSemaphore(DIRECTORY, ID_SEM_EST_AGV, CANTIDAD_AGVS);
        semEstAGV.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC sem_estado_AGV destruido");
        
        semMemAGV_5.getSemaphore(DIRECTORY, ID_SEM_MEM_AGV_5, CANTIDAD_AGVS);
        semMemAGV_5.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_AGV_5 destruido");
        
        semMemCanastos.getSemaphore(DIRECTORY, ID_SEM_MEM_CANASTOS_10, CANTIDAD_AGVS);
        semMemCanastos.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_Canastos destruido");
        
        
        IPC::Semaphore semEstadoRobot5("semEstadoRobot5");
        semEstadoRobot5.getSemaphore(DIRECTORY, ID_SEM_EST_ROBOT_5, CANTIDAD_AGVS);
        semEstadoRobot5.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC sem_estado_robot_5 destruido");
        
        IPC::Semaphore semMemEstadoRobot5("semMemEstadoRobot5");
        semMemEstadoRobot5.getSemaphore(DIRECTORY, ID_SEM_MEM_EST_ROBOT_5, 1);
        semMemEstadoRobot5.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_EstadoRobot_5 destruido");

        IPC::Semaphore semMemCinta6("semMemCinta6");
        IPC::Semaphore semRobot11("semRobot11");
        IPC::Semaphore semRobot12("semRobot12");
        semMemCinta6.getSemaphore(DIRECTORY, ID_SEM_MEM_CINTA_6, CANTIDAD_CINTAS_6);
        semMemCinta6.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_Cinta_6 destruido");
        
        semRobot11.getSemaphore(DIRECTORY, ID_SEM_ROBOT_11, CANTIDAD_CINTAS_6);
        semRobot11.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC sem_robot_11 destruido");
        
        semRobot12.getSemaphore(DIRECTORY, ID_SEM_ROBOT_12, CANTIDAD_CINTAS_6);
        semRobot12.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC sem_robot_12 destruido");
        
        // Do the same with the queues
        PedidosAGVMessageQueue colaPedidosAGV;
        colaPedidosAGV.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
        colaPedidosAGV.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC colaPedidosAGV destruido");

        PedidosCanastosMessageQueue colaPedidosCanastos;
        colaPedidosCanastos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_A_AGV);
        colaPedidosCanastos.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC colaPedidosCanastos destruido");

        Barrera1112MessageQueue cola11121;
        Barrera1112MessageQueue cola11122;
        cola11121.getMessageQueue(DIRECTORY,ID_COLA_11_12_1);
        cola11121.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC cola11121 destruido");
        
        cola11122.getMessageQueue(DIRECTORY,ID_COLA_11_12_2);
        cola11122.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC cola11122 destruido");
        
        Barrera1112MessageQueue cola12111;
        Barrera1112MessageQueue cola12112;
        cola12111.getMessageQueue(DIRECTORY,ID_COLA_12_11_1);
        cola12111.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC cola12111 destruido");
        
        cola12112.getMessageQueue(DIRECTORY,ID_COLA_12_11_2);
        cola12112.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC cola12112 destruido");
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, 
        e.get_error_description().c_str());
        return -1;
    }
}

