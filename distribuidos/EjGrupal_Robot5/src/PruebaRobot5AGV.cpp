/* 
 * File:   PruebaRobot5AGV.cpp
 * Author: nebla
 *
 * Created on November 1, 2013, 10:49 PM
 */

#include <cstdlib>

#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "PedidosAgvMessageQueue.h"
#include "./IPC/Semaphore/Semaphore.h"
#include "BufferCanastoSharedMemory.h"

int main(int argc, char** argv) {

    char buffer[TAM_BUFFER];
    if (argc != 2) {
        sprintf(buffer, "Prueba Robot 5 - AGV: Error en la cant de parametros, falto ID del agv.\n");
        write(fileno(stdout), buffer, strlen(buffer));
        exit(-1);
    }
    int idAgv;
    sscanf(argv[1], "%d", &idAgv);
    if ((idAgv < 0) || (idAgv > 2)) {
        sprintf(buffer, "Prueba Robot 5 - AGV: Error el id del Agv debe estar entre 0 y 2.\n");
        write(fileno(stdout), buffer, strlen(buffer));
        exit(-1);
    }

    /* Creo los buffer de donde recolectar los canastos */
    BufferCanastoSharedMemory bufferCanasto[3];
    bufferCanasto[0].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_0);
    bufferCanasto[1].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_1);
    bufferCanasto[2].getSharedMemory(DIRECTORY, ID_BUFFER_CANASTOS_2);

    /* Creo los semaforos de acceso a los buffer de los canastos */
    Semaphore semaforoAccesoBufferAgv;
    semaforoAccesoBufferAgv.getSemaphore(DIRECTORY, ID_SEM_ACCESO_BUFFER_AGV, 3);
    
    /* Creo los semaforos de bloqueo de los Agv */
    Semaphore semaforoBloqueoAgv;
    semaforoBloqueoAgv.getSemaphore(DIRECTORY, ID_SEM_BLOQUEO_AGV, 3);

    /* Creo la cola de pedidos de los AGV */
    PedidosAgvMessageQueue colaPedidosAgv;
    colaPedidosAgv.getMessageQueue(DIRECTORY, ID_COLA_PEDIDOS_AGV_5);

    bool deboSeguir = true;
    while (deboSeguir) {
        try {
            /* Simulo la legada de un agv, el mismo envia un pedido por un canasto */

            PedidoCanasto pedidoCanasto;
            pedidoCanasto.idAgv = idAgv;
            pedidoCanasto.tipoPieza = PIEZA_1;

            MensajePedidoAgv nuevoPedido;
            nuevoPedido.mtype = TIPO_PEDIDO_CANASTO;
            nuevoPedido.pedidoCanasto = pedidoCanasto;

            sprintf(buffer, "AGV %d: Enviando pedido por canasto.\n", idAgv);
            write(fileno(stdout), buffer, strlen(buffer));
            colaPedidosAgv.enviarPedidoAgv(nuevoPedido);

            /* Una vez enviado el pedido, me bloqueo esperando por el mismo */
            semaforoBloqueoAgv.wait(idAgv);

            /* Cuando me libero significa que el pedido ya fue resuelto */

            /* Accedo al buffer */
            semaforoAccesoBufferAgv.wait(idAgv);
            {
                Canasto *canasto = bufferCanasto[idAgv - 1].readInfo();
                sprintf(buffer, "AGV %d: Canasto recibido con piezas: %d\n", idAgv, (*canasto).cantidadPiezas);
                write(fileno(stdout), buffer, strlen(buffer));
            }
            semaforoAccesoBufferAgv.signal(idAgv);

            /* Me duermo un intervalo de tiempo aleatorio antes de realizar un nuevo pedido */
            srand(time(NULL));
            int delay = (rand() % 10) + 1;
            sprintf(buffer, "AGV %d: Durmiendo durante %d\n", idAgv, delay);
            write(fileno(stdout), buffer, strlen(buffer));
            sleep(delay);
        }
        catch (IPCException const& ex) {
            sprintf(buffer, "AGV %d: Error: %s\n", idAgv, ex.what());
            write(fileno(stderr), buffer, strlen(buffer));
            exit(-1);
        }
    }

    return 0;
}