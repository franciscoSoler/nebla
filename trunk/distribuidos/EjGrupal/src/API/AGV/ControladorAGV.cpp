/* 
 * File:   ControladorAGV.cpp
 * Author: francisco
 * 
 * Created on 26 de octubre de 2013, 20:26
 */

#include "ControladorAGV.h"
#include "../../Logger/Logger.h"

ControladorAGV::ControladorAGV() {
}

ControladorAGV::~ControladorAGV() {
}

void ControladorAGV::iniciarControlador(int id_AGV) {
    try {
        Logger::getInstance();
        sprintf(this->buffer, "AGV-%u:", id_AGV + 1);
        Logger::setProcessInformation(this->buffer);

        this->id_AGV = id_AGV;

        this->semBloqueoAGV = IPC::Semaphore("semBloqueoAGV");
        this->semBloqueoAGV.getSemaphore(DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, 3);

        this->semBloqueoRobot5 = IPC::Semaphore("semBloqueoRobot5");
        this->semBloqueoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);

        this->semMemEstadoRobot5 = IPC::Semaphore("semMemEstadoRobot5");
        this->semMemEstadoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);

        this->semBufferAGV_5 = IPC::Semaphore("semBufferAGV_5");
        this->semBufferAGV_5.getSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, 3);

        this->semMemCanastos = IPC::Semaphore("semMemCanastos");
        this->semMemCanastos.getSemaphore(DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, 3);

        this->shMemEstadoRobot5 = IPC::EstadoRobot5SharedMemory("shMemEstadoRobot5");
        this->shMemEstadoRobot5.getSharedMemory(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);

        this->colaPedidosCanastos = IPC::PedidosCanastosMessageQueue("colaPedidosCanastos");
        this->colaPedidosCanastos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_A_AGV);

        this->colaPedidosAGV = IPC::PedidosAgvMessageQueue("colaPedidosAGV");
        this->colaPedidosAGV.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_AGV_5);

        this->semRobotCinta = IPC::Semaphore("semRobotCinta");
        this->shMemCanastos = IPC::BufferCanastosSharedMemory("shMemCanastos");
        this->shMemPasajeCanastoEntre5yAGV = IPC::BufferCanastoEntre5yAGVSharedMemory("shMemPasajeCanastoEntre5yAGV");
        switch (id_AGV) {
            case 0 : 
                this->semRobotCinta.getSemaphore(DIRECTORY, ID_SEM_ROBOT_11, 2);
                this->shMemCanastos.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_1);
                this->shMemPasajeCanastoEntre5yAGV.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_1);
                break;
            case 1 :
                this->semRobotCinta.getSemaphore(DIRECTORY, ID_SEM_ROBOT_12, 2);
                this->shMemCanastos.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_2);
                this->shMemPasajeCanastoEntre5yAGV.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_2);
                break;
            case 2 :
                this->semRobotCinta.getSemaphore(DIRECTORY, ID_SEM_ROBOT_11, 2);
                this->shMemCanastos.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_3);
                this->shMemPasajeCanastoEntre5yAGV.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTO_AGV_5_3);
                break;
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

TipoPieza ControladorAGV::atenderPedidos() {
    try {
        sprintf(this->buffer, "AGV-%u - atenderPedidos:", id_AGV + 1);
        Logger::setProcessInformation(this->buffer);
        
        PedidoCanasto pedidoCanasto;
        Logger::logMessage(Logger::TRACE, "recibo pedidos canastos de la cola");

        this->colaPedidosCanastos.recibirPedidoCanasto(this->id_AGV + 1, &pedidoCanasto);
        this->posicionCanasto = pedidoCanasto.lugar;
        //this->pedidoDeDeposito = pedidoCanasto.pedidoEsDeDeposito;
        return pedidoCanasto.tipoPieza;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

Canasto ControladorAGV::buscarPieza(TipoPieza tipoPieza) {
    try {
        sprintf(this->buffer, "AGV-%u - buscarPieza:", id_AGV + 1);
        Logger::setProcessInformation(this->buffer);
        EstadoRobot5 estadoRobot5;
        Canasto canasto;

        Logger::logMessage(Logger::TRACE, "realizo el pedido del canasto al robot 5");
        
        MensajePedidoAgv_5 pedidoCanastoAGV;
        pedidoCanastoAGV.mtype = 1;
        pedidoCanastoAGV.idAgv = this->id_AGV;
        pedidoCanastoAGV.tipo = tipoPieza;
        this->colaPedidosAGV.enviarPedidoAGV(pedidoCanastoAGV);

        // despierto robot 5 si esta durmiendo
        Logger::logMessage(Logger::TRACE, "veo si robot 5 esta durmiendo");
        this->semMemEstadoRobot5.wait();
        this->shMemEstadoRobot5.readInfo(&estadoRobot5);
        if (estadoRobot5.robot5Durmiendo) {
            Logger::logMessage(Logger::TRACE, "robot 5 esta durmiendo, lo despierto");
            estadoRobot5.robot5Durmiendo = false;
            this->shMemEstadoRobot5.writeInfo(&estadoRobot5);
            this->semBloqueoRobot5.signal();
        }
        this->semMemEstadoRobot5.signal();

        Logger::logMessage(Logger::TRACE, "duermo hasta que robot 5 traiga el canasto");
        this->semBloqueoAGV.wait(this->id_AGV);

        Logger::logMessage(Logger::TRACE, "busco el canasto traido por robot 5");
        this->semBufferAGV_5.wait(this->id_AGV);
        this->shMemPasajeCanastoEntre5yAGV.readInfo(&canasto);
        this->semBufferAGV_5.signal(this->id_AGV);

        return canasto;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorAGV::reponerCanasto(Canasto canasto) {
    try {
        sprintf(this->buffer, "AGV-%u - reponerCanasto:", id_AGV + 1);
        Logger::setProcessInformation(this->buffer);
        BufferCanastos canastos;

        Logger::logMessage(Logger::TRACE, "voy a obtener memoria de los canastos");
        this->semMemCanastos.wait(this->id_AGV);
        Logger::logMessage(Logger::TRACE, "obtengo memoria de los canastos");
        
        this->shMemCanastos.readInfo(&canastos);

        canastos.canastos[this->posicionCanasto] = canasto;

        // no me importa si el pedido es del deposito!!!
        /*
        if (this->pedidoDeDeposito) {
            // esta mal, deberia despertar robots si estan durmiedno espernado a uqe se rellene esto!!!!
            Logger::logMessage(Logger::TRACE, "el pedido vino del deposito");
            this->shMemCanastos.writeInfo(&canastos);
            this->semMemCanastos.signal(this->id_AGV);
            //return;
        }*/
        if (this->posicionCanasto == canastos.robotCinta1EsperaPorElLugarNumero) {
            Logger::logMessage(Logger::TRACE, "despierto al robot de la cinta 1");
            canastos.robotCinta1EsperaPorElLugarNumero = -1;
            this->semRobotCinta.signal(0);
        }
        if (this->posicionCanasto == canastos.robotCinta2EsperaPorElLugarNumero) {
            Logger::logMessage(Logger::TRACE, "despierto al robot de la cinta 2");
            canastos.robotCinta2EsperaPorElLugarNumero = -1;
            this->semRobotCinta.signal(1);
        }

        this->shMemCanastos.writeInfo(&canastos);
        this->semMemCanastos.signal(this->id_AGV);
        return;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}