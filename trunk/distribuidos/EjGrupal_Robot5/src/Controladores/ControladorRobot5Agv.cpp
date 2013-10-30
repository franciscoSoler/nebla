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

#include "../common.h"

void iniciarIPC(ComunicacionRobot5MessageQueue &colaComunicacionRobot5,
        PedidosAgvMessageQueue &colaPedidos, 
        BufferCanastoSharedMemory *bufferCanasto, 
        Semaphore *semaforoAccesoBufferAgv, 
        Semaphore *semaforoAgv) {
    
    /* Obtengo la cola de comunicacion con el robot 5 */
    colaComunicacionRobot5 = ComunicacionRobot5MessageQueue();
    colaComunicacionRobot5.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
    
    /* Obtengo la cola de pedidos */
    colaPedidos = PedidosAgvMessageQueue();
    colaPedidos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
              
    /* Obtengo el buffer para depositar los canastos */    
    bufferCanasto[0] = BufferCanastoSharedMemory();
    bufferCanasto[0].getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_1);
    
    bufferCanasto[1] = BufferCanastoSharedMemory();
    bufferCanasto[1].getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_2);
    
    bufferCanasto[2] = BufferCanastoSharedMemory();
    bufferCanasto[2].getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_3);
    
    /* Obtengo los semaforos de acceso a los buffer */    
    semaforoAccesoBufferAgv[0] = Semaphore();
    semaforoAccesoBufferAgv[0].getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_CANASTOS_1, 1);
            
    semaforoAccesoBufferAgv[1] = Semaphore();
    semaforoAccesoBufferAgv[1].getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_CANASTOS_2, 1);
            
    semaforoAccesoBufferAgv[2] = Semaphore();
    semaforoAccesoBufferAgv[2].getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_CANASTOS_3, 1);
    
    /* Obtengo los semaforos de bloqueo de los Agv*/
    semaforoAgv[0] = Semaphore();
    semaforoAgv[0].getSemaphore(DIRECTORY, ID_SEM_AGV_1, 1);
 
    semaforoAgv[1] = Semaphore();
    semaforoAgv[1].getSemaphore(DIRECTORY, ID_SEM_AGV_2, 1);
 
    semaforoAgv[2] = Semaphore();
    semaforoAgv[2].getSemaphore(DIRECTORY, ID_SEM_AGV_3, 1);
}

int main(int argc, char** argv) {

    /* Creamos e iniciamos todos los ipc necesarios para
     * el funcionamiento del proceso.
     */
    ComunicacionRobot5MessageQueue colaComunicacionRobot5;
    
    PedidosAgvMessageQueue colaPedidos;
        
    BufferCanastoSharedMemory bufferCanasto[3];
    Semaphore semaforoAccesoBufferAgv[3];
    Semaphore semaforoAgv[3];
    
    try {
        iniciarIPC(colaComunicacionRobot5, colaPedidos, bufferCanasto, semaforoAccesoBufferAgv, semaforoAgv);
    }
    catch (std::exception ex) {
        char buffer[TAM_BUFFER];
        sprintf (buffer, "Controlador Robot 5 - AGV:Error: %s\n", ex.what());
        write (fileno(stderr),buffer, strlen(buffer));
        exit(-1);
    }
   
    bool deboSeguir = true;
    
    while (deboSeguir) {
        /* Obtengo un pedido de alguno de los AGV */
        PedidoAgv *nuevoPedido;
        colaPedidos.recibirPedidoAgv(TIPO_PEDIDO_CANASTO, nuevoPedido);
        
        /* Le envio el pedido al robot 5 Aplicacion */
        PedidoRobot5 pedidoRobot5;
        pedidoRobot5.pedidoCanasto = (*nuevoPedido).pedidoCanasto;
        pedidoRobot5.tipo = PEDIDO_CANASTO;

        MensajePedidoRobot5 mensajePedidoRobot5;
        mensajePedidoRobot5.mtype = TIPO_PEDIDO_ROBOT_5;
        mensajePedidoRobot5.pedidoRobot5 = pedidoRobot5;
           
        colaComunicacionRobot5.enviarPedidoRobot5(mensajePedidoRobot5);
        
        /* Me quedo esperando la respuesta del robot 5 */
        MensajeRespuestaCanasto mensajeRespuestaCanasto;
        colaComunicacionRobot5.recibirCanasto(TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5, &mensajeRespuestaCanasto);
        
        /* Una vez recibido el canasto requerido, se lo envio al agv correspondiente.
         * Intento acceder al buffer del agv al que va destinado al pedido.  
         */
        semaforoAccesoBufferAgv[mensajeRespuestaCanasto.idAgv - 1].wait();
    
        /* Una vez obtenido al acceso, dejo el canasto. */
        bufferCanasto[mensajeRespuestaCanasto.idAgv - 1].writeInfo(&mensajeRespuestaCanasto.canasto);
    
        /* Libero al AGV para que este retire el canasto */
        semaforoAgv[mensajeRespuestaCanasto.idAgv - 1].signal();
    
        /* Libero el buffer del canasto */
        semaforoAccesoBufferAgv[mensajeRespuestaCanasto.idAgv - 1].signal();
    
        
    }
    
    return 0;
}

