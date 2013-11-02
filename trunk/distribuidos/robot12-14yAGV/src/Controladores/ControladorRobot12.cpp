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
        this->semRobot12 = IPC::Semaphore("semRobot12");
        this->semRobot12.getSemaphore(DIRECTORY, ID_SEM_ROBOT_12, 2);

        this->semMemCinta6 = IPC::Semaphore("semMemCinta6");
        this->semMemCinta6.getSemaphore(DIRECTORY, ID_SEM_MEM_CINTA_6, 2);

        this->semMemCanastos = IPC::Semaphore("semMemCanastos");
        this->semMemCanastos.getSemaphore(DIRECTORY, ID_SEM_MEM_CANASTOS_10, 3);

        this->colaPedidosCanastos = PedidosCanastosMessageQueue();
        this->colaPedidosCanastos.getMessageQueue(DIRECTORY,ID_COLA_PEDIDOS_A_AGV);

        this->shMemCanastos = BufferCanastoSharedMemory();
        this->shMemCanastos.getSharedMemory(DIRECTORY, ID_MEM_BUFFER_CANASTOS_2);

        this->shMemCinta6 = Cinta6SharedMemory();

        this->cola1112 = Barrera1112MessageQueue();
        this->cola1211 = Barrera1112MessageQueue();
        if (numRobot == 0) {
            this->shMemCinta6.getSharedMemory(DIRECTORY, ID_MEM_CINTA_6_1);
            this->cola1112.getMessageQueue(DIRECTORY,ID_COLA_11_12_1);
            this->cola1211.getMessageQueue(DIRECTORY,ID_COLA_12_11_1);
        } else {
            this->shMemCinta6.getSharedMemory(DIRECTORY, ID_MEM_CINTA_6_2);
            this->cola1112.getMessageQueue(DIRECTORY,ID_COLA_11_12_2);
            this->cola1211.getMessageQueue(DIRECTORY,ID_COLA_12_11_2);
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
        MessageBarrera messageBarrera;
        // version 2 (sin semaforo!!!)
        this->cola1112.receive(0, &messageBarrera);

        Logger::logMessage(Logger::TRACE, "voy a tomar la memoria de la cinta 6");
        this->semMemCinta6.wait(this->id_Robot);
        Logger::logMessage(Logger::TRACE, "tome la memoria de la cinta 6");
        
        this->shMemCinta6.readInfo(&ctrlCinta);
        especifProductoAux = ctrlCinta.especificacionesProd[ctrlCinta.puntoLectura];
        
        Logger::logMessage(Logger::TRACE, "devuelvo la mem de la cinta 6");
        this->semMemCinta6.signal(this->id_Robot);
        return especifProductoAux;

       /* while (true) {
            this->semMemCinta6.wait(this->id_Robot);
            &ctrlCinta = this->shMemCinta6.readInfo();

            if (ctrlCinta.cantLibres == BUFF_SIZE_CINTA_6) {
                // aviso que me duermo y me duermo
                ctrlCinta.robot12Durmiendo = true;
                this->shMemCinta6.writeInfo(&ctrlCinta);
                this->semMemCinta6.signal(this->id_Robot);
                semRobot12.wait(this->id_Robot);
            } else {
                this->semMemCinta6.signal(this->id_Robot);
                break;
            }
        }

        while(true) {
            if(!ctrlCinta.lugarVacio[ctrlCinta.puntoLectura]) {
                return ctrlCinta.especificacionesProd[ctrlCinta.puntoLectura];
            } else {
                this->cola1112.receive(0, &messageBarrera);

                this->semMemCinta6.wait(this->id_Robot);
                &ctrlCinta = shMemCinta6.readInfo();    
                this->semMemCinta6.signal(this->id_Robot);
            }
        }*/
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
            this->semMemCanastos.wait(this->id_semMemCanastos);
            Logger::logMessage(Logger::TRACE, "tome la memoria de los canastos");
            
            this->shMemCanastos.readInfo(&canastos);
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
                this->shMemCanastos.writeInfo(&canastos);

                this->semMemCanastos.signal(this->id_semMemCanastos);
                return false;  
            } else {
                Logger::logMessage(Logger::TRACE, "canasto no presente, aviso que posicion espero, devuelvo mem canastos y me duermo");
                if (this->id_Robot == 0)
                    canastos.robotCinta1EsperaPorElLugarNumero = this->posicionPieza;
                else
                    canastos.robotCinta2EsperaPorElLugarNumero = this->posicionPieza;
                this->shMemCanastos.writeInfo(&canastos);
                this->semMemCanastos.signal(this->id_semMemCanastos);

                this->semRobot12.wait(this->id_Robot);
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
        this->shMemCanastos.readInfo(&canastos);
        
        sprintf(this->buffer, "la cantidad de piezas que hay en el canasto son %d", canastos.canastos[this->posicionPieza].cantidadPiezas);
        Logger::logMessage(Logger::TRACE, this->buffer);
        int piezasRetiradas = std::min(rand() % 3 + 1, canastos.canastos[this->posicionPieza].cantidadPiezas);
        canastos.canastos[this->posicionPieza].cantidadPiezas -= piezasRetiradas;
        this->shMemCanastos.writeInfo(&canastos);
        this->semMemCanastos.signal(this->id_semMemCanastos);

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
            PedidoCanasto pedidoCanasto;
            pedidoCanasto.mtype = this->id_Agv;
            //pedidoCanasto.pedidoEsDeDeposito = false;
            pedidoCanasto.lugar = this->posicionPieza;
            pedidoCanasto.tipoPieza = tipoPieza;
            this->colaPedidosCanastos.enviarPedidoCanasto(pedidoCanasto);
        }    
        Logger::logMessage(Logger::TRACE, "me duermo");
        this->semRobot12.wait(this->id_Robot);
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
        MessageBarrera message;
        message.mtype = 1;
        //version 2
        this->cola1211.send(message);
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