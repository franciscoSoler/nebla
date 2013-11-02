/* 
 * File:   TerminatorRobot5.cpp
 * Author: knoppix
 *
 * Created on November 1, 2013, 11:51 PM
 */

#include <cstdlib>

#include "IPC/Semaphore/Semaphore.h"

#include "IPC/MessageQueue/PedidosAgvMessageQueue.h"
#include "IPC/MessageQueue/PedidosProduccionMessageQueue.h"
#include "IPC/MessageQueue/ComunicacionRobot5MessageQueue.h"

#include "IPC/SharedMemory/EstadoRobot5SharedMemory.h"
#include "IPC/SharedMemory/EstadoRobot11SharedMemory.h"
#include "IPC/SharedMemory/BufferCanastoSharedMemory.h"
#include "IPC/SharedMemory/CintaTransportadoraSharedMemory.h"

int main(int argc, char** argv) {
    char buffer[TAM_BUFFER];
    
    try {
        /* Obtengo la cola de comunicacion con el robot 5 con los dos procesos del controlador */
        ComunicacionRobot5MessageQueue colaComunicacionRobot5 = ComunicacionRobot5MessageQueue();
        colaComunicacionRobot5.getMessageQueue(DIRECTORY,ID_COLA_API_ROBOT_5);
        colaComunicacionRobot5.destroy();
    }
    catch (IPCException const& ex) {       
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what()); 
        write(fileno(stderr), buffer, strlen(buffer));  
        exit(-1); 
    }
    
    try {
        /* Obtengo la cola de pedidos del robot 5 */
        PedidosProduccionMessageQueue colaPedidos = PedidosProduccionMessageQueue();
        colaPedidos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_PRODUCCION);
        colaPedidos.destroy();
    }
    catch (IPCException const& ex) {     
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what());
        write(fileno(stderr), buffer, strlen(buffer));   
        exit(-1); 
    }
    
    /*
     * ROBOT 5 - AGV
     */
    
    try {
        /* Obtengo los buffer para depositar los canastos */    
        BufferCanastoSharedMemory bufferCanasto[3]; 
        bufferCanasto[0].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_0);
        bufferCanasto[0].destroy();
        bufferCanasto[1].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_1);
        bufferCanasto[1].destroy();
        bufferCanasto[2].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_2);
        bufferCanasto[2].destroy();
    }
    catch (IPCException const& ex) {     
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what()); 
        write(fileno(stderr), buffer, strlen(buffer));  
        exit(-1); 
    }
    
    try {
        /* Obtengo los semaforos de acceso a los buffer de los canastos */    
        Semaphore semaforoAccesoBufferAgv;
        semaforoAccesoBufferAgv.getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_AGV, 3);
        semaforoAccesoBufferAgv.destroy();
    }
    catch (IPCException const& ex) {      
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what());    
        write(fileno(stderr), buffer, strlen(buffer));  
        exit(-1); 
    }
    
    try {
        /* Obtengo los semaforos de bloqueo de los Agv */
        Semaphore semaforoBloqueoAgv;
        semaforoBloqueoAgv.getSemaphore(DIRECTORY, ID_SEM_BLOQUEO_AGV, 3);
        semaforoBloqueoAgv.destroy();
    }
    catch (IPCException const& ex) {       
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what());    
        write(fileno(stderr), buffer, strlen(buffer));    
        exit(-1); 
    }

    try {
        /* Obtengo la cola de pedidos de los AGV */    
        PedidosAgvMessageQueue colaPedidosAgv; 
        colaPedidosAgv.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
        colaPedidosAgv.destroy();
    }
    catch (IPCException const& ex) {        
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what());
        write(fileno(stderr), buffer, strlen(buffer));  
        exit(-1); 
    }
    
    /*
     * Robot 5 - Cinta
     */
    
    try {
        /* Obtengo las memorias compartidas y los semaforos de las cintas transportadoras */
        Semaphore semCinta0 = Semaphore();
        semCinta0.getSemaphore(DIRECTORY, ID_CINTA_6_0, 1);
        semCinta0.destroy();
        CintaTransportadoraSharedMemory memCinta0 = CintaTransportadoraSharedMemory();
        memCinta0.getSharedMemory(DIRECTORY, ID_CINTA_6_0);
        memCinta0.destroy();
    
        Semaphore semCinta1 = Semaphore();
        semCinta1.getSemaphore(DIRECTORY, ID_CINTA_6_1, 1);
        semCinta1.destroy();
        CintaTransportadoraSharedMemory memCinta1 = CintaTransportadoraSharedMemory();
        memCinta1.getSharedMemory(DIRECTORY, ID_CINTA_6_1);
        memCinta1.destroy();
    }
    catch (IPCException const& ex) {        
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what());
        write(fileno(stderr), buffer, strlen(buffer));        
        exit(-1); 
    }
    
    try {
        /* Obtengo el semaforo de bloqueo del robot 5 */
        Semaphore semBloqueo5 = Semaphore();
        semBloqueo5.getSemaphore(DIRECTORY, ID_SEM_BLOQUEO_ROBOT_5, 1);
        semBloqueo5.destroy();
       
        /* Obtengo la memoria de estado del robot 5 y el semaforo de acceso */
        EstadoRobot5SharedMemory estadoRobot5 = EstadoRobot5SharedMemory();
        estadoRobot5.getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_5);
        estadoRobot5.destroy();
        Semaphore semaforoAccesoEstadoRobot5 = Semaphore();
        semaforoAccesoEstadoRobot5.getSemaphore(DIRECTORY, ID_SEM_ACCESO_ESTADO_ROBOT_5, 1);
        semaforoAccesoEstadoRobot5.destroy();
    }
    catch (IPCException const& ex) {        
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what());        
        write(fileno(stderr), buffer, strlen(buffer));        
        exit(-1); 
    }
    
    try {
        /* Obtengo la memoria de estado de los robot 11 y los semaforos de acceso */
        EstadoRobot11SharedMemory estadoRobot11[2];
        estadoRobot11[0].getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_11_0);
        estadoRobot11[0].destroy();
        estadoRobot11[1].getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_11_1);
        estadoRobot11[1].destroy();
    
        Semaphore semaforoAccesoEstadoRobot11 = Semaphore();
        semaforoAccesoEstadoRobot11.getSemaphore(DIRECTORY, ID_SEM_ACCESO_ESTADO_ROBOT_11, 2);
        semaforoAccesoEstadoRobot11.destroy();
    
        /* Obtengo los semaforos de bloqueo del robot 11 */
        Semaphore semaforoBloqueoRobot11 = Semaphore();
        semaforoBloqueoRobot11.getSemaphore(DIRECTORY, ID_SEM_BLOQUEO_ROBOT_11, 2);
        semaforoBloqueoRobot11.destroy();
    }
    catch (IPCException const& ex) {
        sprintf(buffer, "Terminator Robot 5:Error: %s\n", ex.what());
        write(fileno(stderr), buffer, strlen(buffer));
        exit(-1);
    }       
    
    return 0;
}

