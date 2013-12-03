/* 
 * File:   ControladorAGV.cpp
 * Author: francisco
 * 
 * Created on 26 de octubre de 2013, 20:26
 */

#include "ControladorAGV.h"
#include "../../Logger/Logger.h"
#include <Comunicaciones/Objects/MiddlewareAPI.h>
 

ControladorAGV::ControladorAGV() {
}

ControladorAGV::~ControladorAGV() {
}

void ControladorAGV::iniciarControlador(int id_AGV) {
    try {
        Logger::getInstance();
        sprintf(this->buffer, "AGV-%u:", id_AGV);
        Logger::setProcessInformation(this->buffer);

        MiddlewareAPI middleware;
        middleware.crearCanales(id_AGV + 1, ID_TIPO_AGV);

        this->id_AGV = id_AGV;

        this->semBloqueoAGV = IPC::Semaphore("semBloqueoAGV");
        this->semBloqueoAGV.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BLOQUEO_AGV, 3);

        this->semBufferAGV_5 = IPC::Semaphore("semBufferAGV_5");
        this->semBufferAGV_5.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BUFFER_AGV_5, 3);

        this->semMemCanastos = IPC::Semaphore("semMemCanastos");
        this->semMemCanastos.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, 3);

        this->colaPedidosCanastos = IPC::PedidosCanastosMessageQueue("colaPedidosCanastos", this->id_AGV, ID_TIPO_AGV);
        this->colaPedidosCanastos.getMessageQueue((char*) DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);

        this->colaPedidosAGV_5 = IPC::PedidosAgvMessageQueue("colaPedidosAGV_5", this->id_AGV + 1, ID_TIPO_ROBOT5_AGV, ID_TIPO_AGV);
        this->colaPedidosAGV_5.getMessageQueue((char*) DIRECTORY_AGV, ID_COLA_PEDIDOS_AGV_5);

        this->semRobotCinta = IPC::Semaphore("semRobotCinta");
        
        this->shMemBuffer5yAGV = IPC::BufferCanastoEntre5yAGVSharedMemory("shMemBuffer5yAGV");
        switch (id_AGV) {
            case 0 : 
                this->semRobotCinta.getSemaphore((char*) DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, 2);
                this->shMemBufferCanastos = IPC::BufferCanastosSharedMemory("shMemBufferCanastos_0");
                this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_0);
                this->shMemBuffer5yAGV.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_AGV_5_0);
                break;
            case 1 :
                this->semRobotCinta.getSemaphore((char*) DIRECTORY_ROBOT_12, ID_SEM_BLOQUEO_ROBOT_12, 2);
                this->shMemBufferCanastos = IPC::BufferCanastosSharedMemory("shMemBufferCanastos_1");
                this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_1);
                this->shMemBuffer5yAGV.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_AGV_5_1);
                break;
            case 2 :
                this->semRobotCinta.getSemaphore((char*) DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, 2);
                this->shMemBufferCanastos = IPC::BufferCanastosSharedMemory("shMemBufferCanastos_2");
                this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_2);
                this->shMemBuffer5yAGV.getSharedMemory((char*)DIRECTORY_AGV, ID_BUFFER_AGV_5_2);
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
        sprintf(this->buffer, "AGV-%u - atenderPedidos:", id_AGV);
        Logger::setProcessInformation(this->buffer);
        
        MensajePedidoRobotCinta_6 pedidoCanasto;

        this->colaPedidosCanastos.recibirPedidoCanasto(this->id_AGV + 1, &pedidoCanasto);
        Logger::logMessage(Logger::TRACE, "recibi un pedido para reponer un canasto");
        this->posicionCanasto = pedidoCanasto.pedidoCanastoAgv.lugar;
        //this->pedidoDeDeposito = pedidoCanasto.pedidoEsDeDeposito;
        return pedidoCanasto.pedidoCanastoAgv.tipoPieza;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

Canasto ControladorAGV::buscarPieza(TipoPieza tipoPieza) {
    try {
        sprintf(this->buffer, "AGV-%u - buscarPieza:", id_AGV);
        Logger::setProcessInformation(this->buffer);
        // EstadoRobot5 estadoRobot5;
        Canasto canasto;

        MensajePedidoAgv_5 pedidoCanastoAGV;
        pedidoCanastoAGV.pedidoCanastoAgv.idAgv = this->id_AGV;
        pedidoCanastoAGV.pedidoCanastoAgv.tipoPieza = tipoPieza;
        this->colaPedidosAGV_5.enviarPedidoAgv(TIPO_PEDIDO_CANASTO, pedidoCanastoAGV);

        Logger::logMessage(Logger::TRACE, "realice el pedido del canasto al robot 5, duermo hasta que lo traiga");
        this->semBloqueoAGV.wait(this->id_AGV);

        this->semBufferAGV_5.wait(this->id_AGV);
        this->shMemBuffer5yAGV.readInfo(&canasto);
        this->semBufferAGV_5.signal(this->id_AGV);
        
        sprintf(this->buffer, "el canasto traido por robot 5 tiene la pieza %d, cantidad %d", canasto.tipoPieza, canasto.cantidadPiezas);
        Logger::logMessage(Logger::TRACE, this->buffer);

        return canasto;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorAGV::reponerCanasto(Canasto canasto) {
    try {
        sprintf(this->buffer, "AGV-%u - reponerCanasto:", id_AGV);
        Logger::setProcessInformation(this->buffer);
        BufferCanastos canastos;

        this->semMemCanastos.wait(this->id_AGV);
        this->shMemBufferCanastos.readInfo(&canastos);
        
        canastos.canastos[this->posicionCanasto] = canasto;
        
        sprintf(this->buffer, "guardo el canasto en la posicion %d, con la pieza %d, cantidad %d en la cinta %d", this->posicionCanasto, canasto.tipoPieza, canasto.cantidadPiezas, id_AGV);
        Logger::logMessage(Logger::IMPORTANT, this->buffer);
       
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
        
        if (canastos.robotCinta1EsperaPorElLugarNumero == -2) {
            canastos.robotCinta1EsperaPorElLugarNumero = -1;
            this->semRobotCinta.signal(0);
        }
        if (canastos.robotCinta2EsperaPorElLugarNumero == -2) {
            canastos.robotCinta2EsperaPorElLugarNumero = -1;
            this->semRobotCinta.signal(1);
        }

        this->shMemBufferCanastos.writeInfo(&canastos);
        this->semMemCanastos.signal(this->id_AGV);
        return;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}
