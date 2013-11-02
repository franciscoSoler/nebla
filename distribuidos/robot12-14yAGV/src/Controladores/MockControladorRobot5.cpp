/* 
 * File:   MockControladorRobot5.cpp
 * Author: francisco
 * 
 * Created on 31 de octubre de 2013, 16:14
 */

#include "MockControladorRobot5.h"
#include "../Logger/Logger.h"

MockControladorRobot5::MockControladorRobot5() {
}

MockControladorRobot5::~MockControladorRobot5() {
}

void MockControladorRobot5::iniciarControlador() {
    try {
        srand (getpid());
        Logger::getInstance();
        sprintf(this->buffer, "Robot 5:");
        Logger::setProcessInformation(this->buffer);
        
        /* Obtengo el semaforo de sincronizacion de Robot11 con AGVs y robot 5*/
        this->semEstadoRobot5 = IPC::Semaphore("semEstadoRobot5");
        this->semEstadoRobot5.getSemaphore(DIRECTORY, ID_SEM_EST_ROBOT_5, 1);
        
        this->semMemEstadoRobot5 = IPC::Semaphore("semMemEstadoRobot5");
        this->semMemEstadoRobot5.getSemaphore(DIRECTORY, ID_SEM_MEM_EST_ROBOT_5, 1);
        
        this->semEstAGV = IPC::Semaphore("semEstadoAGV");
        this->semEstAGV.getSemaphore(DIRECTORY, ID_SEM_EST_AGV, 3);

        this->semMemAGV_5 = IPC::Semaphore("semMemAGV_5");
        this->semMemAGV_5.getSemaphore(DIRECTORY, ID_SEM_MEM_AGV_5, 3);

        this->semRobot11 = IPC::Semaphore("semRobot11");
        this->semRobot11.getSemaphore(DIRECTORY, ID_SEM_ROBOT_11, 2);

        this->semMemCinta6 = IPC::Semaphore("semMemCinta6");
        this->semMemCinta6.getSemaphore(DIRECTORY, ID_SEM_MEM_CINTA_6, 2);

        this->shMemEstadoRobot5 = EstadoRobot5SharedMemory();
        this->shMemEstadoRobot5.getSharedMemory(DIRECTORY, ID_MEM_ESTADO_ROBOT_5);

        this->shMemCinta61 = Cinta6SharedMemory();
        this->shMemCinta62 = Cinta6SharedMemory();
        this->shMemCinta61.getSharedMemory(DIRECTORY, ID_MEM_CINTA_6_1);
        this->shMemCinta62.getSharedMemory(DIRECTORY, ID_MEM_CINTA_6_2);
        
        this->shMemPasajeCanastoEntre5yAGV1 = BufferCanastoEntre5yAGVSharedMemory();
        this->shMemPasajeCanastoEntre5yAGV2 = BufferCanastoEntre5yAGVSharedMemory();
        this->shMemPasajeCanastoEntre5yAGV3 = BufferCanastoEntre5yAGVSharedMemory();
        this->shMemPasajeCanastoEntre5yAGV1.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_1);
        this->shMemPasajeCanastoEntre5yAGV2.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_2);
        this->shMemPasajeCanastoEntre5yAGV3.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_3);
        
        this->colaPedidosAGV = PedidosAGVMessageQueue();
        this->colaPedidosAGV.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void MockControladorRobot5::atenderPedidoAGV() {
    try {
        sprintf(this->buffer, "Robot 5 - atenderPedidoAGV:");
        Logger::setProcessInformation(this->buffer);
        PedidoCanastoAGV pedidoAGV;
        BufferCanastoEntre5yAGVSharedMemory shMemCanasto;
        Canasto canasto;

        Logger::logMessage(Logger::TRACE, "recibo pedido de AGV");
        this->colaPedidosAGV.recibirPedidoAGV(0, &pedidoAGV);
        
        
        sprintf(this->buffer, "busco %d del AGV Nº %d",pedidoAGV.tipo, pedidoAGV.idAgv);
        Logger::logMessage(Logger::TRACE, this->buffer);
        canasto.cantidadPiezas = 100;
        canasto.tipoPieza = pedidoAGV.tipo;
        usleep(100);
        
        Logger::logMessage(Logger::TRACE, "voy a obtener mem buffer entre robot5 y agv");
        this->semMemAGV_5.wait(pedidoAGV.idAgv);
        Logger::logMessage(Logger::TRACE, "obtengo mem buffer entre robot5 y agv");
        if (pedidoAGV.idAgv == 0)
            shMemCanasto = this->shMemPasajeCanastoEntre5yAGV1;
        else if (pedidoAGV.idAgv == 1)
            shMemCanasto = this->shMemPasajeCanastoEntre5yAGV2;
        else 
            shMemCanasto = this->shMemPasajeCanastoEntre5yAGV3;
        shMemCanasto.writeInfo(&canasto);
        this->semMemAGV_5.signal(pedidoAGV.idAgv);
        
        sprintf(this->buffer, "despierto el AGV Nº %d", pedidoAGV.idAgv);
        Logger::logMessage(Logger::TRACE, this->buffer);
        this->semEstAGV.signal (pedidoAGV.idAgv);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void MockControladorRobot5::llenarCinta() {
    try {
        sprintf(this->buffer, "Robot 5 - llenarCinta:");
        Logger::setProcessInformation(this->buffer);
        Cinta6SharedMemory shMemcinta6;
        CintaTransportadora_6 cinta6;
        EstadoRobot5 estado5;
        EspecifProd especificacionesProd;
        EspecifPiezaDeProd especifPantalla;
        especifPantalla.cantidad = 2;
        especifPantalla.tipoPieza = PANTALLA_1;
        
        EspecifPiezaDeProd especifPieza;
        especifPieza.cantidad = 2;
        especifPieza.tipoPieza = PIEZA_1;
        especificacionesProd.cantPiezas = 1;
        especificacionesProd.idProducto = ID_PROD1;
        especificacionesProd.numOrdenCompra = 3;
        especificacionesProd.tipoPantalla = especifPantalla;
        especificacionesProd.pieza[0] = especifPieza;
        
        ProductoEnProduccion prod;
        prod.nroOrdenCompra = 3;
        prod.gabinete = GABINETE_1;
        
        int numCinta = rand() % 2;
        sprintf(this->buffer, "voy a obtener mem cinta 6-%d", numCinta + 1);
        Logger::logMessage(Logger::TRACE, this->buffer);
        this->semMemCinta6.wait(numCinta);
        sprintf(this->buffer, "obtengo mem cinta 6-%d", numCinta + 1);
        Logger::logMessage(Logger::TRACE, this->buffer);
        if (numCinta == 0)
            shMemcinta6 = this->shMemCinta61;
        else
            shMemcinta6 = this->shMemCinta62;
        
        shMemcinta6.readInfo(&cinta6);
        
        if (!cinta6.lugarVacio[cinta6.puntoEscritura]) {
            Logger::logMessage(Logger::TRACE, "lugar lleno, aviso que me duermo y me duermo");
            this->semMemEstadoRobot5.wait();
            this->shMemEstadoRobot5.readInfo(&estado5);
            estado5.robot5Durmiendo = true;
            this->shMemEstadoRobot5.writeInfo(&estado5);
            this->semMemEstadoRobot5.signal();
            
            this->semMemCinta6.signal(numCinta);
            this->semEstadoRobot5.wait();
        } else {
            cinta6.lugarVacio[cinta6.puntoEscritura] = false;
            cinta6.cantLibres--;
            cinta6.especificacionesProd[cinta6.puntoEscritura] = especificacionesProd;
            cinta6.gabinetes[cinta6.puntoEscritura] = prod;
            if (cinta6.robot11Durmiendo) {
                Logger::logMessage(Logger::TRACE, "robot 11 durmiendo, lo despierto");
                cinta6.robot11Durmiendo = false;
                this->semRobot11.signal(numCinta);
            }
            Logger::logMessage(Logger::TRACE, "pongo gabinete y devuelvo la cinta");
            shMemcinta6.writeInfo(&cinta6);
            this->semMemCinta6.signal(numCinta);
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}