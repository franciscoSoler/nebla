/* 
 * File:   ControladorRobot5Agv.cpp
 * Author: nebla
 *
 * Created on October 27, 2013, 7:55 PM
 */

#include <cstdlib>

#include <exception>

#include "../IPC/MessageQueue/ComunicacionRobot5MessageQueue.h"
#include "../IPC/SharedMemory/BufferCanastoSharedMemory.h"
#include "../IPC/MessageQueue/PedidosAgvMessageQueue.h"
#include "../IPC/Semaphore/Semaphore.h"

#include "../IPC/IPCException.h"

#include "../common.h"

void iniciarIPC(ComunicacionRobot5MessageQueue &colaComunicacionRobot5,
        PedidosAgvMessageQueue &colaPedidos, 
        BufferCanastoSharedMemory *bufferCanasto, 
        Semaphore *semaforoAccesoBufferAgv, 
        Semaphore *semaforoAgv) {
    
    /* Obtengo la cola de comunicacion con el robot 5 */
    colaComunicacionRobot5.getMessageQueue(DIRECTORY,ID_COLA_API_ROBOT_5);
    
    /* Obtengo la cola de pedidos */
    colaPedidos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
              
    /* Obtengo el buffer para depositar los canastos */    
    bufferCanasto[0].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_0);
    bufferCanasto[1].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_1);
    bufferCanasto[2].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_2);
    
    /* Obtengo los semaforos de acceso a los buffer */    
    semaforoAccesoBufferAgv[0].getSemaphore(DIRECTORY, ID_BUFFER_CANASTOS_0, 1);
    semaforoAccesoBufferAgv[1].getSemaphore(DIRECTORY, ID_BUFFER_CANASTOS_1, 1);
    semaforoAccesoBufferAgv[2].getSemaphore(DIRECTORY, ID_BUFFER_CANASTOS_2, 1);
    
    /* Obtengo los semaforos de bloqueo de los Agv */
    semaforoAgv[0].getSemaphore(DIRECTORY, ID_SEM_AGV_1, 1);
    semaforoAgv[1].getSemaphore(DIRECTORY, ID_SEM_AGV_2, 1);
    semaforoAgv[2].getSemaphore(DIRECTORY, ID_SEM_AGV_3, 1);
}

int main(int argc, char** argv) {

    /* Se crean e inician todos los ipc necesarios para
     * el funcionamiento del proceso.
     */
    ComunicacionRobot5MessageQueue colaComunicacionRobot5 = ComunicacionRobot5MessageQueue();
    
    PedidosAgvMessageQueue colaPedidos = PedidosAgvMessageQueue();
        
    BufferCanastoSharedMemory bufferCanasto[3];
    Semaphore semaforoAccesoBufferAgv[3];
    Semaphore semaforoAgv[3];
    
    try {
        iniciarIPC(colaComunicacionRobot5, colaPedidos, bufferCanasto, semaforoAccesoBufferAgv, semaforoAgv);
    }
    catch (IPCException const& ex) {
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
            semaforoAccesoBufferAgv[mensajeRespuestaCanasto.idAgv - 1].wait();
            {
                sprintf(buffer, "Controlador Robot 5 - AGV: Accedo al buffer del canasto para el AGV: %d.\n",mensajeRespuestaCanasto.idAgv);
                write(fileno(stdout), buffer, strlen(buffer));

                /* Una vez obtenido al acceso, dejo el canasto. */
                bufferCanasto[mensajeRespuestaCanasto.idAgv - 1].writeInfo(&mensajeRespuestaCanasto.canasto);
            }
            /* Libero al AGV para que este retire el canasto */
            semaforoAgv[mensajeRespuestaCanasto.idAgv - 1].signal();
            
            sprintf(buffer, "Controlador Robot 5 - AGV: Libero al AGV %d.\n",mensajeRespuestaCanasto.idAgv);
            write(fileno(stdout), buffer, strlen(buffer));

            /* Libero el buffer del canasto */
            semaforoAccesoBufferAgv[mensajeRespuestaCanasto.idAgv - 1].signal();
        }
        catch (IPCException const& ex) {
            char buffer[TAM_BUFFER];
            sprintf(buffer, "Controlador Robot 5 - AGV:Error: %s\n", ex.what());
            write(fileno(stderr), buffer, strlen(buffer));
            exit(-1);
        }
    }
    
    return 0;
}

