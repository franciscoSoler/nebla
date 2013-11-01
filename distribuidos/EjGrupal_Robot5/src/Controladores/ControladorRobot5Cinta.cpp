/* 
 * File:   ControladorRobot5Cinta.cpp
 * Author: nebla
 *
 * Created on October 31, 2013, 9:17 PM
 */

#include <cstdlib>
#include <exception>

#include "../CintaTransportadoraInicial.h"

#include "../IPC/SharedMemory/EstadoRobot5SharedMemory.h"
#include "../IPC/SharedMemory/EstadoRobot11SharedMemory.h"

#include "../IPC/MessageQueue/PedidosProduccionMessageQueue.h"
#include "../IPC/MessageQueue/ComunicacionRobot5MessageQueue.h"

#include "../common.h"

void iniciarIPC(ComunicacionRobot5MessageQueue &colaComunicacionRobot5,
        PedidosProduccionMessageQueue &colaPedidos, 
        CintaTransportadoraInicial *cintaTransportadora,
        
        EstadoRobot5SharedMemory &estadoRobot5,
        Semaphore &semaforoAccesoEstadoRobot5,
        Semaphore &semaforoBloqueoRobot5,
        
        EstadoRobot11SharedMemory *estadoRobot11,
        Semaphore &semaforoAccesoEstadoRobot11,
        Semaphore &semaforoBloqueoRobot11) {
    
    /* Obtengo la cola de comunicacion con el robot 5 */
    colaComunicacionRobot5.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
    
    /* Obtengo la cola de pedidos */
    colaPedidos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_PRODUCCION);
              
    /* Obtengo las cintas transportadoras */    
    cintaTransportadora[0] = CintaTransportadoraInicial(0);
    cintaTransportadora[0].iniciarCinta(ID_MEM_CINTA_6_0, ID_SEM_ACCESO_CINTA_6_0);
    
    cintaTransportadora[1] = CintaTransportadoraInicial(1);
    cintaTransportadora[1].iniciarCinta(ID_MEM_CINTA_6_1, ID_SEM_ACCESO_CINTA_6_1);

    /* Obtengo la memoria compartida del estado del robot 5 y su semaforo de acceso */
    estadoRobot5.getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_5);
    semaforoAccesoEstadoRobot5.getSemaphore(DIRECTORY, ID_SEM_ACCESO_ESTADO_ROBOT_5);
    semaforoBloqueoRobot5.getSemaphore(DIRECTORY, ID_SEM_BLOQUEO_ROBOT_5, 1);

    /* Obtengo la memoria compartida del estado de los robot 11 y sus semaforos de acceso */
    semaforoBloqueoRobot11.getSemaphore(DIRECTORY, ID_SEM_BLOQUEO_ROBOT_11, 2);
    semaforoAccesoEstadoRobot11.getSemaphore(DIRECTORY, ID_SEM_ACCESO_ESTADO_ROBOT_11, 2);
    estadoRobot11[0].getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_11_0);
    estadoRobot11[1].getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_11_1);
    
}

void leerEstadoCintas (CintaTransportadoraInicial *cintaTransportadora,
                        EstadoCinta &estadoCinta0,
                        EstadoCinta &estadoCinta1) {
    
    estadoCinta0 = cintaTransportadora[0].obtenerEstadoCinta();
    estadoCinta1 = cintaTransportadora[1].obtenerEstadoCinta();
}

int main(int argc, char** argv) {

     /* Se crean e inician todos los ipc necesarios para
      * el funcionamiento del proceso.
      */
    ComunicacionRobot5MessageQueue colaComunicacionRobot5 = ComunicacionRobot5MessageQueue();
    
    CintaTransportadoraInicial cintasTransportadoras[2];
    PedidosProduccionMessageQueue colaPedidosProduccion = PedidosProduccionMessageQueue();    
    
    EstadoRobot5SharedMemory estadoRobot5 = EstadoRobot5SharedMemory();
    Semaphore semaforoAccesoEstadoRobot5 = Semaphore();
    Semaphore semaforoBloqueoRobot5 = Semaphore();
    
    EstadoRobot11SharedMemory estadoRobot11[2];
    Semaphore semaforoAccesoEstadoRobot11 = Semaphore();
    Semaphore semaforoBloqueoRobot11 = Semaphore();
    
    try {
        iniciarIPC(colaComunicacionRobot5, 
                colaPedidosProduccion, 
                cintasTransportadoras,
                
                estadoRobot5,
                semaforoAccesoEstadoRobot5,
                semaforoBloqueoRobot5,
                
                estadoRobot11,
                semaforoAccesoEstadoRobot11,
                semaforoBloqueoRobot11);
    }
    catch (IPCException const& ex) {
        char buffer[TAM_BUFFER];
        sprintf (buffer, "Controlador Robot 5 - Cinta: Error: %s\n", ex.what());
        write (fileno(stderr),buffer, strlen(buffer));
        exit(-1);
    }
    
    EstadoCinta estadoCinta0;
    EstadoCinta estadoCinta1;
    
    bool deboSeguir = true;

    while (deboSeguir) {

        /* Recibo una orden de produccion */
        MensajePedidoProduccion *mensajePedidoProduccion;
        colaPedidosProduccion.recibirPedidoProduccion(TIPO_PEDIDO_PRODUCCION, mensajePedidoProduccion);

        bool ultimo = false;

        /* Sigo depositando gabinetes mientras no reciba el ultimo de este pedido */
        while (!ultimo) {         
            /* Obtengo los estados de ambas cintas para intentar depositar el primer gbainete */
            leerEstadoCintas(cintasTransportadoras, estadoCinta0, estadoCinta1);
            bool bloqueadas = (estadoCinta0.ocupado && estadoCinta1.ocupado);
            while (bloqueadas) {
                /* Como ambas cintas tienen su primer lugar ocupado, 
                 * aviso que me voy a bloquear y me bloqueo */
                semaforoAccesoEstadoRobot5.wait();
                {
                    bool *bloqueado;
                    (*bloqueado) = true;
                    estadoRobot5.writeInfo(bloqueado);
                }
                semaforoAccesoEstadoRobot5.signal();
                semaforoBloqueoRobot5.wait();
                /* Cuando me libero debo verificar que se haya liberado 
                 * alguna de las cintas*/
                leerEstadoCintas(cintasTransportadoras, estadoCinta0, estadoCinta1);
                bloqueadas = (estadoCinta0.ocupado && estadoCinta1.ocupado);
            }

            /* A partir de aca, al menos una de las cintas tiene lugar para 
             * depositar un gabinete. 
             * Por lo tanto le indico al robot 5 que busque uno. */
            


        }

    }
    
    return 0;
}