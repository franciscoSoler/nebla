/* 
 * File:   ControladorRobot5Agv.cpp
 * Author: nebla
 *
 * Created on October 27, 2013, 7:55 PM
 */

#include <cstdlib>

#include <exception>

#include "IPCs/Duran/MessageQueue/ComunicacionRobot5MessageQueue.h"
#include "IPCs/Duran/SharedMemory/BufferCanastoSharedMemory.h"
#include "IPCs/Duran/MessageQueue/PedidosAgvMessageQueue.h"
#include "IPCs/Semaphore/Semaphore.h"

#include "Exceptions/Exception.h"

#include "Common.h"

void iniciarIPC(IPC::ComunicacionRobot5MessageQueue &colaComunicacionRobot5,
        IPC::PedidosAgvMessageQueue &colaPedidos, 
        IPC::BufferCanastoSharedMemory *bufferCanasto, 
        IPC::Semaphore &semaforoAccesoBufferAgv, 
        IPC::Semaphore &semaforoBloqueoAgv) {
    
    /* Obtengo la cola de comunicacion con el robot 5 */
    colaComunicacionRobot5.getMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_API_ROBOT_5);
    
    /* Obtengo la cola de pedidos */
    colaPedidos.getMessageQueue(DIRECTORY_AGV,ID_COLA_PEDIDOS_AGV_5);

    /* Obtengo el buffer para depositar los canastos */    
    bufferCanasto[0].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_0);
    bufferCanasto[1].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_1);
    bufferCanasto[2].getSharedMemory(DIRECTORY_AGV, ID_BUFFER_AGV_5_2);
    
    /* Obtengo los semaforos de acceso a los buffer */    
    semaforoAccesoBufferAgv.getSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, 3);
    
    /* Obtengo los semaforos de bloqueo de los Agv */
    semaforoBloqueoAgv.getSemaphore(DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, 3);
}

int main(int argc, char** argv) {

    /* Se crean e inician todos los ipc necesarios para
     * el funcionamiento del proceso.
     */
    IPC::ComunicacionRobot5MessageQueue colaComunicacionRobot5 = IPC::ComunicacionRobot5MessageQueue("ComunicacionRobot5MessageQueue");
    
    IPC::PedidosAgvMessageQueue colaPedidos = IPC::PedidosAgvMessageQueue("PedidosAgvMessageQueue");

    IPC::BufferCanastoSharedMemory bufferCanasto[CANTIDAD_AGVS];
    for (int i = 0; i < CANTIDAD_AGVS; ++i) {
        char buffer[TAM_BUFFER];
        sprintf(buffer, "BufferCanastoSharedMemory %d", i);
        bufferCanasto[i] = IPC::BufferCanastoSharedMemory(buffer);
    }
    IPC::Semaphore semaforoAccesoBufferAgv = IPC::Semaphore("Acceso al buffer AGV - 5");
    IPC::Semaphore semaforoBloqueoAgv = IPC::Semaphore("Bloqueo AGV");
    
    try {
        iniciarIPC(colaComunicacionRobot5, colaPedidos, bufferCanasto, semaforoAccesoBufferAgv, semaforoBloqueoAgv);
    }
    catch (Exception const& ex) {
        char buffer[TAM_BUFFER];
        sprintf (buffer, "Controlador Robot 5 - AGV: Error: %s\n", ex.what());
        write (fileno(stderr),buffer, strlen(buffer));
        exit(-1);
    }
    
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Controlador Robot 5 - AGV: Iniciando.\n");
    write(fileno(stdout), buffer, strlen(buffer));
    
    bool deboSeguir = true;
    
    while (deboSeguir) {
        try {
            /* Obtengo un pedido de alguno de los AGV */
            sprintf(buffer, "Controlador Robot 5 - AGV: Esperando un pedido por parte de un AGV.\n");
            write(fileno(stdout), buffer, strlen(buffer));
            
            MensajePedidoAgv *nuevoPedido;
            colaPedidos.recibirPedidoAgv(TIPO_PEDIDO_CANASTO, nuevoPedido);
            
            sprintf(buffer, "Controlador Robot 5 - AGV: Recibió un pedido de un AGV.\n");
            write(fileno(stdout), buffer, strlen(buffer));
            
            /* Le envio el pedido al robot 5 Aplicacion */
            PedidoRobot5 pedidoRobot5;
            pedidoRobot5.pedidoCanasto = (*nuevoPedido).pedidoCanasto;
            pedidoRobot5.tipo = PEDIDO_CANASTO;

            MensajePedidoRobot5 mensajePedidoRobot5;
            mensajePedidoRobot5.mtype = TIPO_PEDIDO_ROBOT_5;
            mensajePedidoRobot5.pedidoRobot5 = pedidoRobot5;

            sprintf(buffer, "Controlador Robot 5 - AGV: Le envio un pedido de canasto al robot 5.\n");
            write(fileno(stdout), buffer, strlen(buffer));
            colaComunicacionRobot5.enviarPedidoRobot5(mensajePedidoRobot5);

            /* Me quedo esperando la respuesta del robot 5 */
            MensajeRespuestaCanasto mensajeRespuestaCanasto;
            colaComunicacionRobot5.recibirCanasto(TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5, &mensajeRespuestaCanasto);
            sprintf(buffer, "Controlador Robot 5 - AGV: Recibió la respuesta del robot 5 para el AGV %d.\n",mensajeRespuestaCanasto.idAgv);
            write(fileno(stdout), buffer, strlen(buffer));
                       
            /* Una vez recibido el canasto requerido, se lo envio al agv correspondiente.
             * Intento acceder al buffer del agv al que va destinado al pedido.  
             */
            semaforoAccesoBufferAgv.wait(mensajeRespuestaCanasto.idAgv);
            {
                sprintf(buffer, "Controlador Robot 5 - AGV: Accedo al buffer del canasto para el AGV: %d.\n",mensajeRespuestaCanasto.idAgv);
                write(fileno(stdout), buffer, strlen(buffer));

                /* Una vez obtenido al acceso, dejo el canasto. */
                bufferCanasto[mensajeRespuestaCanasto.idAgv - 1].writeInfo(&mensajeRespuestaCanasto.canasto);
            }
            /* Libero al AGV para que este retire el canasto */
            semaforoBloqueoAgv.signal(mensajeRespuestaCanasto.idAgv);
            
            sprintf(buffer, "Controlador Robot 5 - AGV: Libero al AGV %d.\n",mensajeRespuestaCanasto.idAgv);
            write(fileno(stdout), buffer, strlen(buffer));

            /* Libero el buffer del canasto */
            semaforoAccesoBufferAgv.signal(mensajeRespuestaCanasto.idAgv);
        }
        catch (Exception const& ex) {
            char buffer[TAM_BUFFER];
            sprintf(buffer, "Controlador Robot 5 - AGV:Error: %s\n", ex.what());
            write(fileno(stderr), buffer, strlen(buffer));
            exit(-1);
        }
    }
    
    return 0;
}