/* 
 * File:   LauncherRobot5.cpp
 * Author: nebla
 *
 * Created on November 1, 2013, 10:14 PM
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

void createIpc() {

    /* Creo la cola de comunicacion con el robot 5 con los dos procesos del controlador */
    ComunicacionRobot5MessageQueue colaComunicacionRobot5 = ComunicacionRobot5MessageQueue();
    colaComunicacionRobot5.createMessageQueue(DIRECTORY,ID_COLA_API_ROBOT_5);

    /* Creo la cola de pedidos del robot 5 */
    PedidosProduccionMessageQueue colaPedidos = PedidosProduccionMessageQueue();
    colaPedidos.createMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_PRODUCCION);

    /*
     * ROBOT 5 - AGV
     */
    
    /* Creo los buffer para depositar los canastos */    
    BufferCanastoSharedMemory bufferCanasto[3]; 
    bufferCanasto[0].createSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_0);
    bufferCanasto[1].createSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_1);
    bufferCanasto[2].createSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_2);
    
    /* Creo los semaforos de acceso a los buffer de los canastos */    
    Semaphore semaforoAccesoBufferAgv[3];
    semaforoAccesoBufferAgv[0].createSemaphore(DIRECTORY, ID_BUFFER_CANASTOS_0, 1);
    semaforoAccesoBufferAgv[0].initializeSemaphore(0,1);
    semaforoAccesoBufferAgv[1].createSemaphore(DIRECTORY, ID_BUFFER_CANASTOS_1, 1);
    semaforoAccesoBufferAgv[1].initializeSemaphore(0,1);
    semaforoAccesoBufferAgv[2].createSemaphore(DIRECTORY, ID_BUFFER_CANASTOS_2, 1);
    semaforoAccesoBufferAgv[2].initializeSemaphore(0,1);
    
    /* Creo los semaforos de bloqueo de los Agv */
    Semaphore semaforoAgv[3];
    semaforoAgv[0].createSemaphore(DIRECTORY, ID_SEM_AGV_1, 1);
    semaforoAgv[0].initializeSemaphore(0,0);
    semaforoAgv[1].createSemaphore(DIRECTORY, ID_SEM_AGV_2, 1);
    semaforoAgv[1].initializeSemaphore(0,0);
    semaforoAgv[2].createSemaphore(DIRECTORY, ID_SEM_AGV_3, 1);
    semaforoAgv[2].initializeSemaphore(0,0);

    /* Creo la cola de pedidos de los AGV */    
    PedidosAgvMessageQueue colaPedidosAgv; 
    colaPedidosAgv.createMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
    
    /*
     * Robot 5 - Cinta
     */
    
    /* Creo las memorias compartidas y los semaforos de las cintas transportadoras */
    Semaphore semCinta0 = Semaphore();
    semCinta0.createSemaphore(DIRECTORY, ID_CINTA_6_0, 1);
    semCinta0.initializeSemaphore(0,1);
    CintaTransportadoraSharedMemory memCinta0 = CintaTransportadoraSharedMemory();
    memCinta0.createSharedMemory(DIRECTORY, ID_CINTA_6_0);
    
    Semaphore semCinta1 = Semaphore();
    semCinta1.createSemaphore(DIRECTORY, ID_CINTA_6_1, 1);
    semCinta1.initializeSemaphore(0,1);
    CintaTransportadoraSharedMemory memCinta1 = CintaTransportadoraSharedMemory();
    memCinta1.createSharedMemory(DIRECTORY, ID_CINTA_6_1);

    /* Creo el semaforo de bloqueo del robot 5 */
    Semaphore semBloqueo5 = Semaphore();
    semBloqueo5.createSemaphore(DIRECTORY, ID_SEM_BLOQUEO_ROBOT_5, 1);
    semBloqueo5.initializeSemaphore(0,0);
    
    /* Creo la memoria de estado del robot 5 y el semaforo de acceso */
    EstadoRobot5SharedMemory estadoRobot5 = EstadoRobot5SharedMemory();
    estadoRobot5.createSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_5);
    Semaphore semaforoAccesoEstadoRobot5 = Semaphore();
    semaforoAccesoEstadoRobot5.createSemaphore(DIRECTORY, ID_SEM_ACCESO_ESTADO_ROBOT_5, 1);
    semaforoAccesoEstadoRobot5.initializeSemaphore(0,1);
    
    /* Creo la memoria de estado de los robot 11 y los semaforos de acceso */
    EstadoRobot11SharedMemory estadoRobot11[2];
    estadoRobot11[0].createSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_11_0);
    estadoRobot11[1].createSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_11_1);
    
    Semaphore semaforoAccesoEstadoRobot11 = Semaphore();
    semaforoAccesoEstadoRobot11.createSemaphore(DIRECTORY, ID_SEM_ACCESO_ESTADO_ROBOT_11, 2);
    semaforoAccesoEstadoRobot11.initializeSemaphore(0,1);
    semaforoAccesoEstadoRobot11.initializeSemaphore(1,1);
    
    /* Creo los semaforos de bloqueo del robot 11 */
    Semaphore semaforoBloqueoRobot11 = Semaphore();
    semaforoBloqueoRobot11.createSemaphore(DIRECTORY, ID_SEM_BLOQUEO_ROBOT_11, 2);
    semaforoBloqueoRobot11.initializeSemaphore(0,0);
    semaforoBloqueoRobot11.initializeSemaphore(1,0);
}

int main(int argc, char** argv) {

    createIpc();
    
    char buffer[TAM_BUFFER];
            
    /* Lanzo el controlador Robot 5-AGV */
    pid_t pid = fork();
    if (pid == 0) {
        execlp("./controladorRobot5Agv", "controladorRobot5Agv", (char *) 0);
	sprintf(buffer, "Launcher Robot 5: Error al lanzar el controlador robot 5-AGV: %s\n", strerror(errno));
	write(fileno(stdout), buffer, strlen(buffer));
    }
    
    /* Lanzo el controlador Robot 5-Cinta */
    pid = fork();
    if (pid == 0) {
        execlp("./controladorRobot5Cinta", "controladorRobot5Cinta", (char *) 0);
	sprintf(buffer, "Launcher Robot 5: Error al lanzar el controlador robot 5-Cinta: %s\n", strerror(errno));
	write(fileno(stdout), buffer, strlen(buffer));
    }
    
    /* Lanzo el Robot 5 */
    pid = fork();
    if (pid == 0) {
        execlp("./robot5", "robot5", (char *) 0);
	sprintf(buffer, "Launcher Robot 5: Error al lanzar el Robot 5: %s\n", strerror(errno));
	write(fileno(stdout), buffer, strlen(buffer));
    }
    
    sprintf(buffer, "Launcher Robot 5: Terminado\n");
    write(fileno(stdout), buffer, strlen(buffer));
        
    return 0;
}