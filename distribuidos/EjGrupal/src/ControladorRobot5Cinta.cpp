/* 
 * File:   ControladorRobot5Cinta.cpp
 * Author: nebla
 *
 * Created on October 31, 2013, 9:17 PM
 */

#include <cstdlib>
#include <exception>

#include "API/Robot5/CintaTransportadora/CintaTransportadora6.h"

#include "IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"

#include "IPCs/IPCAbstractos/MessageQueue/ComunicacionRobot5MessageQueue.h"
#include "IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"

#include "Common.h"

void iniciarIPC(IPC::ComunicacionRobot5MessageQueue &colaComunicacionRobot5,
        IPC::PedidosProduccionMessageQueue &colaPedidos, 
        CintaTransportadora6 *cintaTransportadora,
        
        IPC::EstadoRobot5SharedMemory &estadoRobot5,
        IPC::Semaphore &semaforoAccesoEstadoRobot5,
        IPC::Semaphore &semaforoBloqueoRobot5,
        
        IPC::Semaphore &semaforoBloqueoRobot11) {
    
    /* Obtengo la cola de comunicacion con el robot 5 */
    colaComunicacionRobot5.getMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_PEDIDOS_AGV_5);
    
    /* Obtengo la cola de pedidos */
    colaPedidos.getMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_PEDIDOS_PRODUCCION);
              
    /* Obtengo las cintas transportadoras */    
    cintaTransportadora[0] = CintaTransportadora6(0);
    cintaTransportadora[0].iniciarCinta(ID_CINTA_6_0);
    
    cintaTransportadora[1] = CintaTransportadora6(1);
    cintaTransportadora[1].iniciarCinta(ID_CINTA_6_1);

    /* Obtengo la memoria compartida del estado del robot 5 y su semaforo de acceso */
    estadoRobot5.getSharedMemory(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);
    semaforoAccesoEstadoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);
    semaforoBloqueoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);
}

void leerEstadoCintas (CintaTransportadora6 *cintaTransportadora,
                        EstadoCinta &estadoCinta0,
                        EstadoCinta &estadoCinta1) {
    
    estadoCinta0 = cintaTransportadora[0].obtenerEstadoCinta();
    estadoCinta1 = cintaTransportadora[1].obtenerEstadoCinta();
}

int main(int argc, char** argv) {

     /* Se crean e inician todos los ipc necesarios para
      * el funcionamiento del proceso.
      */
    IPC::ComunicacionRobot5MessageQueue colaComunicacionRobot5 = IPC::ComunicacionRobot5MessageQueue("ComunicacionRobot5MessageQueue");

    CintaTransportadora6 cintasTransportadoras[2];
    IPC::PedidosProduccionMessageQueue colaPedidosProduccion = IPC::PedidosProduccionMessageQueue("PedidosProduccionMessageQueue");

    IPC::EstadoRobot5SharedMemory estadoRobot5 = IPC::EstadoRobot5SharedMemory("EstadoRobot5SharedMemory");
    IPC::Semaphore semaforoAccesoEstadoRobot5 = IPC::Semaphore("AccesoEstadoRobot5");
    IPC::Semaphore semaforoBloqueoRobot5 = IPC::Semaphore("BloqueoRobot5");

    IPC::Semaphore semaforoBloqueoRobot11 = IPC::Semaphore("BloqueoRobot11");

    try {
        iniciarIPC(colaComunicacionRobot5, 
                colaPedidosProduccion, 
                cintasTransportadoras,
                estadoRobot5,
                semaforoAccesoEstadoRobot5,
                semaforoBloqueoRobot5,
                semaforoBloqueoRobot11);
    }
    catch (Exception const& ex) {
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

        /* Le envio la orden de producción al robot 5 */
        PedidoRobot5 pedidoRobot5;
        pedidoRobot5.tipo = PEDIDO_PRODUCCION;
        pedidoRobot5.pedidoProduccion = (*mensajePedidoProduccion).pedidoProduccion;

        MensajePedidoRobot5 mensajePedidoRobot5;
        mensajePedidoRobot5.mtype = TIPO_PEDIDO_ROBOT_5;
        mensajePedidoRobot5.pedidoRobot5 = pedidoRobot5;

        /* Le envio el pedido de produccion al robot 5 Aplicacion */
        colaComunicacionRobot5.enviarPedidoRobot5(mensajePedidoRobot5);

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
            pedidoRobot5.tipo = PEDIDO_GABINETE;
            mensajePedidoRobot5.mtype = TIPO_PEDIDO_ROBOT_5;
            mensajePedidoRobot5.pedidoRobot5 = pedidoRobot5;

            /* Le envio el pedido al robot 5 Aplicacion */
            colaComunicacionRobot5.enviarPedidoRobot5(mensajePedidoRobot5);
            
            /* Espero a que me envie el gabinete */
            MensajeRespuestaGabinete mensajeRespuestaGabinete;
            colaComunicacionRobot5.recibirGabinete(TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5, &mensajeRespuestaGabinete);
            ultimo = mensajeRespuestaGabinete.ultimo;
                    
            /* Una vez obtenido el gabinete lo deposito en la cinta que convenga.
             * Si ambas estan libres, en aquella que este menos ocupada *
             * Leo nuevamente el estado de ambas cintas para tener una version mas actualizada del mismo */
            leerEstadoCintas(cintasTransportadoras, estadoCinta0, estadoCinta1);
            
            int cintaAUtilizar;
            if (! estadoCinta0.ocupado && estadoCinta1.ocupado) {
                /* Utilizo la cinta 0 */
                cintaAUtilizar = 0;
            }
            else if (estadoCinta0.ocupado && !estadoCinta1.ocupado) {
                /* Utilizo la cinta 1 */
                cintaAUtilizar = 1;
            }
            else {
                /* Ambas estan libres, utilizo la que menos gabinetes tenga. */
                cintaAUtilizar = (estadoCinta0.cantOcupados <= estadoCinta1.cantOcupados) ? 0 : 1;
            }
            
            /* Una vez obtenida la cinta a utilizar, creo y deposito 
             * el producto en produccion */
            ProductoEnProduccion productoEnProduccion;
            productoEnProduccion.gabinete = mensajeRespuestaGabinete.gabinete;
            productoEnProduccion.nroOrdenCompra = mensajeRespuestaGabinete.ordenDeCompra;
            productoEnProduccion.falla = false;
            cintasTransportadoras[cintaAUtilizar].depositarProductoEnProduccion(productoEnProduccion);
            
            /* Verifico si el robot 11 encargado de la cinta utilizada esta 
             * bloqueado */
            if (cintasTransportadoras[cintaAUtilizar].robot11Bloqueado()) {
                cintasTransportadoras[cintaAUtilizar].marcarRobot11Liberado();
                semaforoBloqueoRobot11.signal(cintaAUtilizar);

            }
        }

    }
    
    return 0;
}