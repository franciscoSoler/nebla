/* 
 * File:   ControladorRobot12.cpp
 * Author: francisco
 * 
 * Created on 24 de octubre de 2013, 22:04
 */

#include "ControladorRobot12.h"
#include "../Logger/Logger.h"
#include "../Parser/Parser.h"

ControladorRobot12::ControladorRobot12() {
}

ControladorRobot12::~ControladorRobot12() {
}

void ControladorRobot12::iniciarControlador(int numRobot) {
    try {
        Logger::getInstance();
        sprintf(this->buffer, "Robot 12-%u:", numRobot);
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

CintaTransportadora_6 ControladorRobot12::esperarProximoGabinete() {
    try {
        sprintf(this->buffer, "Robot 12-%u - esperarProximoGabinete:", this->id_Robot);
        Logger::setProcessInformation(this->buffer);
        
        CintaTransportadora_6 ctrlCinta;
        //EspecifProd especifProductoAux;
        MensajeBarrera messageBarrera;
        
        this->cola11_A_12.receive(0, &messageBarrera);
        Logger::logMessage(Logger::TRACE, "hay gabinete posicionado, comienzo a trabajar");
        
        
        this->semBufferCinta6.wait(this->id_Robot);
        this->shMemBufferCinta6.readInfo(&ctrlCinta);
        this->semBufferCinta6.signal(this->id_Robot);
        return ctrlCinta;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

BufferCanastos ControladorRobot12::obtenerBufferCanastos() {
    try {
        BufferCanastos canastos;
        this->semBufferCanastos.wait(this->id_semMemCanastos);
        this->shMemBufferCanastos.readInfo(&canastos);
        return canastos;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::pedirPiezaFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza) {
    try {
        if (id_Robot == 0) {
            canastos.robotCinta1EsperaPorElLugarNumero = posicionPieza;
            this->posEsperaDelOtroRobot12 = canastos.robotCinta2EsperaPorElLugarNumero;
        } else {
            canastos.robotCinta2EsperaPorElLugarNumero = posicionPieza;
            this->posEsperaDelOtroRobot12 = canastos.robotCinta1EsperaPorElLugarNumero;
        }
        this->shMemBufferCanastos.writeInfo(&canastos);

        this->semBufferCanastos.signal(this->id_semMemCanastos);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::pedirCanastoFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza) {
    try {
        if (id_Robot == 0)
            canastos.robotCinta1EsperaPorElLugarNumero = posicionPieza;
        else
            canastos.robotCinta2EsperaPorElLugarNumero = posicionPieza;
        this->shMemBufferCanastos.writeInfo(&canastos);
        this->semBufferCanastos.signal(this->id_semMemCanastos);

        this->semBloqueoRobot12.wait(id_Robot);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::posicionarCanasto(BufferCanastos canastos) {
    try {
        this->shMemBufferCanastos.writeInfo(&canastos);
        this->semBufferCanastos.signal(this->id_semMemCanastos);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::pedirPiezaAlAGV(TipoPieza tipoPieza, int posicionPieza) {
    try {
        sprintf(this->buffer, "Robot 12-%u - pedirPiezaAlAGV:", this->id_Robot);
        Logger::setProcessInformation(this->buffer);
        
        if (this->posEsperaDelOtroRobot12 != posicionPieza) {
            Logger::logMessage(Logger::TRACE, "el otro robot NO esta esperando la misma pieza, realizo el pedido al agv por la cola");
            MensajePedidoRobotCinta_6 pedidoCanasto;
            pedidoCanasto.mtype = this->id_Agv;
            pedidoCanasto.pedidoCanastoAgv.lugar = posicionPieza;
            pedidoCanasto.pedidoCanastoAgv.tipoPieza = tipoPieza;
            this->colaPedidosCanastos.enviarPedidoCanasto(pedidoCanasto);
        }
        this->semBloqueoRobot12.wait(this->id_Robot);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot12::finalizarEnsamble() {
    try {
        sprintf(this->buffer, "Robot 12-%u - finalizarEnsamble:", this->id_Robot);
        Logger::setProcessInformation(this->buffer);
        MensajeBarrera message;
        message.mtype = 1;
        this->cola12_A_11.send(message);
        Logger::logMessage(Logger::TRACE, "finalizó el ensamble");
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}
