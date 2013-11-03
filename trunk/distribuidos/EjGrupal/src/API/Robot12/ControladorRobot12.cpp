/* 
 * File:   ControladorRobot12.cpp
 * Author: francisco
 * 
 * Created on 24 de octubre de 2013, 22:04
 */

#include "ControladorRobot12.h"
#include "../Logger/Logger.h"

ControladorRobot12::ControladorRobot12() {
}

ControladorRobot12::~ControladorRobot12() {
}

void ControladorRobot12::iniciarControlador(int numRobot) {
    try {
        Logger::getInstance();
        sprintf(this->buffer, "Robot 12-%u:", numRobot + 1);
        Logger::setProcessInformation(this->buffer);
        
        this->id_Agv = 2;
        this->id_semMemCanastos = 1;
        srand (getpid());
        this->id_Robot = numRobot;
        /* Obtengo el semaforo de sincronizacion de Robot12 con AGVs y robot 5*/
        this->semBloqueoRobot12 = IPC::Semaphore("semBloqueoRobot12");
        this->semBloqueoRobot12.getSemaphore((char*) DIRECTORY_ROBOT_12, ID_SEM_BLOQUEO_ROBOT_12, 2);

        this->semBufferCinta6 = IPC::Semaphore("semBufferCinta6");
        this->semBufferCinta6.getSemaphore((char*) DIRECTORY_ROBOT_11, ID_SEM_CINTA_6, 2);

        this->semBufferCanastos = IPC::Semaphore("semMemCanastos");
        this->semBufferCanastos.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, 3);

        this->colaPedidosCanastos = IPC::PedidosCanastosMessageQueue("colaPedidosCanastos");
        this->colaPedidosCanastos.getMessageQueue((char*) DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);

        this->shMemBufferCanastos = IPC::BufferCanastosSharedMemory("shMemBufferCanastos");
        this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_1);

        

        this->cola11_A_12 = IPC::Barrera1112MessageQueue();
        this->cola12_A_11 = IPC::Barrera1112MessageQueue();
        if (numRobot == 0) {
            this->shMemBufferCinta6 = IPC::Cinta6SharedMemory("shMemBufferCinta6_0");
            this->shMemBufferCinta6.getSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_0);
            this->cola11_A_12.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12_1);
            this->cola12_A_11.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11_1);
        } else {
            this->shMemBufferCinta6 = IPC::Cinta6SharedMemory("shMemBufferCinta6_1");
            this->shMemBufferCinta6.getSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_1);
            this->cola11_A_12.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12_2);
            this->cola12_A_11.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11_2);
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

EspecifProd ControladorRobot12::esperarProximoGabinete() {
    try {
        sprintf(this->buffer, "Robot 12-%u - esperarProximoGabinete:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        Logger::logMessage(Logger::TRACE, "espero a que el robot 11 me despierte");
        CintaTransportadora_6 ctrlCinta;
        EspecifProd especifProductoAux;
        MensajeBarrera messageBarrera;
        
        this->cola11_A_12.receive(0, &messageBarrera);

        Logger::logMessage(Logger::TRACE, "voy a tomar la memoria de la cinta 6");
        this->semBufferCinta6.wait(this->id_Robot);
        Logger::logMessage(Logger::TRACE, "tome la memoria de la cinta 6");
        
        this->shMemBufferCinta6.readInfo(&ctrlCinta);
        //TODO robar el archivo de fede que parsea el registro de disco!!!!!!!!!!! lee nomas un archivo
        
        //especifProductoAux = ctrlCinta.especificacionesProd[ctrlCinta.puntoLectura];
        
        Logger::logMessage(Logger::TRACE, "devuelvo la mem de la cinta 6");
        this->semBufferCinta6.signal(this->id_Robot);
        return especifProductoAux;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControladorRobot12::poseePieza(int id_pieza) {
    try {
        sprintf(this->buffer, "Robot 12-%u - poseePieza:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        
        BufferCanastos canastos;
        while (true) {
            
            Logger::logMessage(Logger::TRACE, "voy a tomar la memoria de los canastos");
            this->semBufferCanastos.wait(this->id_semMemCanastos);
            Logger::logMessage(Logger::TRACE, "tome la memoria de los canastos");
            
            this->shMemBufferCanastos.readInfo(&canastos);
            this->buscarPosicionPieza(canastos, id_pieza);
            
            sprintf(this->buffer, "Robot 12-%u - poseePieza:", this->id_Robot + 1);
            Logger::setProcessInformation(this->buffer);

            if (this->posicionPieza != -1) {
                sprintf(this->buffer, "esta el canasto presente en la posicion %d", this->posicionPieza);
                Logger::logMessage(Logger::TRACE, this->buffer);
                if (canastos.canastos[this->posicionPieza].cantidadPiezas != 0)
                    return true;
                
                Logger::logMessage(Logger::TRACE, "no hay piezas en el canasto, aviso que posicion estare esperando y devuelvo mem Canastos");
                // me quede sin piezas en el canasto, aviso por que posicion voy a estar esperando
                if (this->id_Robot == 0) {
                    canastos.robotCinta1EsperaPorElLugarNumero = this->posicionPieza;
                    this->posEsperaDelOtroRobot12 = canastos.robotCinta2EsperaPorElLugarNumero;
                } else {
                    canastos.robotCinta2EsperaPorElLugarNumero = this->posicionPieza;
                    this->posEsperaDelOtroRobot12 = canastos.robotCinta1EsperaPorElLugarNumero;
                }
                this->shMemBufferCanastos.writeInfo(&canastos);

                this->semBufferCanastos.signal(this->id_semMemCanastos);
                return false;  
            } else {
                Logger::logMessage(Logger::TRACE, "canasto no presente, aviso que posicion espero, devuelvo mem canastos y me duermo");
                if (this->id_Robot == 0)
                    canastos.robotCinta1EsperaPorElLugarNumero = this->posicionPieza;
                else
                    canastos.robotCinta2EsperaPorElLugarNumero = this->posicionPieza;
                this->shMemBufferCanastos.writeInfo(&canastos);
                this->semBufferCanastos.signal(this->id_semMemCanastos);

                this->semBloqueoRobot12.wait(this->id_Robot);
            }
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControladorRobot12::agregarConector(EspecifPiezaDeProd piezaDeProd) {
    try {
        sprintf(this->buffer, "Robot 12-%u - agregarConector:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        
        if(!this->poseePieza(piezaDeProd.tipoPieza))
            return false;

        Logger::logMessage(Logger::TRACE, "posee pieza, saco pieza del canasto");
        BufferCanastos canastos;
        this->shMemBufferCanastos.readInfo(&canastos);
        
        sprintf(this->buffer, "la cantidad de piezas que hay en el canasto son %d", canastos.canastos[this->posicionPieza].cantidadPiezas);
        Logger::logMessage(Logger::TRACE, this->buffer);
        int piezasRetiradas = std::min(rand() % 3 + 1, canastos.canastos[this->posicionPieza].cantidadPiezas);
        canastos.canastos[this->posicionPieza].cantidadPiezas -= piezasRetiradas;
        this->shMemBufferCanastos.writeInfo(&canastos);
        this->semBufferCanastos.signal(this->id_semMemCanastos);

        usleep(rand() %100 + 1);
        return true;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::pedirPiezaAlAGV(TipoPieza tipoPieza) {
    try {
        sprintf(this->buffer, "Robot 12-%u - pedirPiezaAlAGV:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        
        if (this->posEsperaDelOtroRobot12 != this->posicionPieza) {
            Logger::logMessage(Logger::TRACE, "el otro robot NO esta esperando la misma pieza, realizo el pedido al agv por la cola");
            MensajePedidoRobotCinta_6 pedidoCanasto;
            pedidoCanasto.mtype = this->id_Agv;
            pedidoCanasto.pedidoCanastoAgv.lugar = this->posicionPieza;
            pedidoCanasto.pedidoCanastoAgv.tipoPieza = tipoPieza;
            this->colaPedidosCanastos.enviarPedidoCanasto(pedidoCanasto);
        }    
        Logger::logMessage(Logger::TRACE, "me duermo");
        this->semBloqueoRobot12.wait(this->id_Robot);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::finalizarEnsamble() {
    try {
        sprintf(this->buffer, "Robot 12-%u - finalizarEnsamble:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        Logger::logMessage(Logger::TRACE, "envio mensaje que finalicé con el ensamble");
        MensajeBarrera message;
        message.mtype = 1;
        //version 2
        this->cola12_A_11.send(message);
        //version 1
        /*this->cola1211.send(message);
        this->cola1112.receive(0, &message);*/
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::buscarPosicionPieza(BufferCanastos canastos, int id_pieza) {
    try { 
        sprintf(this->buffer, "Robot 12-%u - buscarPosicionPieza:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        Logger::logMessage(Logger::TRACE, "busco en que posicion esta la pieza");
        for (int i = 0; i < MAX_QUANTITY_CANASTOS; i++)
            if (canastos.canastos[i].tipoPieza == id_pieza) {
                this->posicionPieza = i;
                return;
            }
        this->posicionPieza = -1;
        return;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}