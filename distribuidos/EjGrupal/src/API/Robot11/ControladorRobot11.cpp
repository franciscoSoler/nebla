/* 
 * File:   ControladorRobot11.cpp
 * Author: francisco
 * 
 * Created on 29 de octubre de 2013, 13:00
 */

#include "ControladorRobot11.h"
#include "../Logger/Logger.h"

ControladorRobot11::ControladorRobot11() {
}

ControladorRobot11::~ControladorRobot11() {
}

void ControladorRobot11::iniciarControlador(int numRobot) {
    try {
        Logger::getInstance();
        sprintf(this->buffer, "Robot 11-%u:", numRobot + 1);
        Logger::setProcessInformation(this->buffer);
        
        this->id_Agv = numRobot * 2 + 1;
        this->id_semMemCanastos = numRobot * 2;
        srand (getpid());
        this->id_Robot = numRobot;
        /* Obtengo el semaforo de sincronizacion de Robot11 con AGVs y robot 5*/
        this->semBloqueoRobot5 = IPC::Semaphore("semBloqueoRobot5");
        this->semBloqueoRobot5.getSemaphore((char*) DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);
        
        this->semMemEstadoRobot5 = IPC::Semaphore("semMemEstadoRobot5");
        this->semMemEstadoRobot5.getSemaphore((char*) DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);

        this->semBloqueoRobot11 = IPC::Semaphore("semBloqueoRobot11");
        this->semBloqueoRobot11.getSemaphore((char*) DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, 2);

        this->semBufferCinta6 = IPC::Semaphore("semBufferCinta6");
        this->semBufferCinta6.getSemaphore((char*) DIRECTORY_ROBOT_11, ID_SEM_CINTA_6, 2);

        this->semMemCanastos = IPC::Semaphore("semMemCanastos");
        this->semMemCanastos.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, 3);

        this->colaPedidosCanastos = IPC::PedidosCanastosMessageQueue("colaPedidosCanastos");
        this->colaPedidosCanastos.getMessageQueue((char*) DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);

        
        
        

        this->shMemEstadoRobot5 = IPC::EstadoRobot5SharedMemory("shMemEstadoRobot5");
        this->shMemEstadoRobot5.getSharedMemory((char*) DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);

        this->shMemBufferCanastos = IPC::BufferCanastosSharedMemory("shMemBufferCanastos");
        
        this->cola11_A_12 = IPC::Barrera1112MessageQueue("cola11_A_12");
        this->cola12_A_11 = IPC::Barrera1112MessageQueue("cola12_A_11");
        if (numRobot == 0) {
            this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_0);
            this->shMemBufferCinta6 = IPC::Cinta6SharedMemory("shMemBufferCinta6_0");
            this->shMemBufferCinta6.getSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_0);
            this->cola11_A_12.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12_1);
            this->cola12_A_11.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11_1);
        } else {
            this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_2);
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

bool ControladorRobot11::buscarProximoGabinete(EspecifProd *piezas) {
    try {
        sprintf(this->buffer, "Robot 11-%u - buscarProximoGabinete:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        
        CintaTransportadora_6 ctrlCinta;
        MensajeBarrera messageBarrera;
        messageBarrera.mtype = 1;

        while (true) {
            sprintf(this->buffer, "voy a obtener mem cinta 6-%d", this->id_Robot);
            Logger::logMessage(Logger::TRACE, this->buffer);
            this->semBufferCinta6.wait(this->id_Robot);
            Logger::logMessage(Logger::TRACE, "obtengo mem cinta 6");
            
            shMemBufferCinta6.readInfo(&ctrlCinta);

            if (ctrlCinta.cantLibres == BUFF_SIZE_CINTA_6) {
                Logger::logMessage(Logger::TRACE, "cinta vacia, aviso que me duermo");
                ctrlCinta.robot11Durmiendo = true;
                this->shMemBufferCinta6.writeInfo(&ctrlCinta);
                this->semBufferCinta6.signal(this->id_Robot);
                
                Logger::logMessage(Logger::TRACE, "devuelvo mem cinta 6 me duermo");
                this->semBloqueoRobot11.wait(this->id_Robot);
            } else {
                Logger::logMessage(Logger::TRACE, "devuelvo mem cinta 6");
                this->semBufferCinta6.signal(this->id_Robot);
                break;
            }
        }
        
        Logger::logMessage(Logger::TRACE, "cinta no vacia, veo si hay gabinete para trabajar");
        sprintf(this->buffer, "punto de lectura = %d", ctrlCinta.puntoLectura);
        Logger::logMessage(Logger::TRACE, this->buffer);

        if(!ctrlCinta.lugarVacio[ctrlCinta.puntoLectura]) {
            Logger::logMessage(Logger::TRACE, "hay gabinete, despierto robot 12");
            this->cola11_A_12.send(messageBarrera);
            //TODO sacar especificaciones piezas del archivo!!!!!!!!!!!!!!!!!!!!!!!!!!
            
            //*piezas = ctrlCinta.especificacionesProd[ctrlCinta.puntoLectura];
            return true;
        }
        Logger::logMessage(Logger::TRACE, "no hay gabinete en el lugar para trabajar");
        return false;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::avanzarCinta() {
    try {
        sprintf(this->buffer, "Robot 11-%u - avanzarCinta:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        CintaTransportadora_6 ctrlCinta;
        EstadoRobot5 estadoRobot5;
        
        Logger::logMessage(Logger::TRACE, "voy a tomar memoria cinta 6");
        this->semBufferCinta6.wait(this->id_Robot);
        Logger::logMessage(Logger::TRACE, "tome memoria cinta 6");
        
        shMemBufferCinta6.readInfo(&ctrlCinta);    

        //ctrlCinta.puntoEscritura = (ctrlCinta.puntoEscritura + 1) % BUFF_SIZE_CINTA_6;
        ctrlCinta.puntoLectura = (ctrlCinta.puntoLectura + 1) % BUFF_SIZE_CINTA_6;
        this->shMemBufferCinta6.writeInfo(&ctrlCinta);
        this->semBufferCinta6.signal(this->id_Robot);

        Logger::logMessage(Logger::TRACE, "voy a tomar memoria estado del robot 5");
        this->semMemEstadoRobot5.wait();
        Logger::logMessage(Logger::TRACE, "tome memoria estado del robot 5");
        
        this->shMemEstadoRobot5.readInfo(&estadoRobot5);
        if (estadoRobot5.robot5Bloqueado) {
            Logger::logMessage(Logger::TRACE, "robot 5 durmiendo, lo despierto");
            estadoRobot5.robot5Bloqueado = false;
            this->shMemEstadoRobot5.writeInfo(&estadoRobot5);
            this->semBloqueoRobot5.signal();
        }
        Logger::logMessage(Logger::TRACE, "devuelvo mem estado robot 5");
        this->semMemEstadoRobot5.signal();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControladorRobot11::agregarPantalla(EspecifPiezaDeProd piezaDeProd) {
    try {
        sprintf(this->buffer, "Robot 11-%u - agregarPantalla:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        
        if(!this->poseePieza(piezaDeProd.tipoPieza))
            return false;
        
        Logger::logMessage(Logger::TRACE, "posee piezas, las retiro del canasto");
        BufferCanastos canastos;
        this->shMemBufferCanastos.readInfo(&canastos);
        
        sprintf(this->buffer, "la cantidad de piezas que hay en el canasto son %d", canastos.canastos[this->posicionPieza].cantidadPiezas);
        Logger::logMessage(Logger::TRACE, this->buffer);
        int piezasRetiradas = std::min(rand() % 3 + 1, canastos.canastos[this->posicionPieza].cantidadPiezas);
        canastos.canastos[this->posicionPieza].cantidadPiezas -= piezasRetiradas;
        this->shMemBufferCanastos.writeInfo(&canastos);
        this->semMemCanastos.signal(this->id_semMemCanastos);

        usleep(rand() %100 + 1);
        return true;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::pedirPiezaAlAGV(TipoPieza tipoPieza) {
    try {
        sprintf(this->buffer, "Robot 11-%u - pedirPiezaAlAGV:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);
        Logger::logMessage(Logger::TRACE, "no podee la pieza, se la pido al agv por la cola");
        
        MensajePedidoRobotCinta_6 pedidoCanasto;
        pedidoCanasto.mtype = this->id_Agv;
        pedidoCanasto.pedidoCanastoAgv;
        pedidoCanasto.pedidoCanastoAgv.lugar = this->posicionPieza;
        pedidoCanasto.pedidoCanastoAgv.tipoPieza = tipoPieza;
        this->colaPedidosCanastos.enviarPedidoCanasto(pedidoCanasto);
        this->semBloqueoRobot11.wait(this->id_Robot);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

Caja ControladorRobot11::cerrarYTomarCaja() {
    try {
        sprintf(this->buffer, "Robot 11-%u - cerrarYTomarCaja:", this->id_Robot + 1);
        Logger::setProcessInformation(this->buffer);

        CintaTransportadora_6 ctrlCinta;
        Caja unaCaja;
        
        Logger::logMessage(Logger::TRACE, "voy a tomar la memoria de la cinta 6 y armo la caja");
        this->semBufferCinta6.wait(this->id_Robot);
        Logger::logMessage(Logger::TRACE, "tome la memoria de la cinta 6 y armo la caja");
        
        shMemBufferCinta6.readInfo(&ctrlCinta);    

        ctrlCinta.cantLibres++;
        ctrlCinta.lugarVacio[ctrlCinta.puntoLectura] = true;
        this->shMemBufferCinta6.writeInfo(&ctrlCinta);
        
        Logger::logMessage(Logger::TRACE, "devuelvo mem cinta 6");
        this->semBufferCinta6.signal(this->id_Robot);

        unaCaja.idProducto = ctrlCinta.productoProduccion[ctrlCinta.puntoLectura].tipoProducto;
        unaCaja.ordenDeCompra = ctrlCinta.productoProduccion[ctrlCinta.puntoLectura].nroOrdenCompra;
        return unaCaja;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::depositarCaja(Caja unaCaja) {
    sprintf(this->buffer, "Robot 11-%u - depositarCaja:", this->id_Robot + 1);
    Logger::setProcessInformation(this->buffer);
    Logger::logMessage(Logger::TRACE, "deposite una caja!!!!!!!!!!!!!!!!");
    //TODO agregar lo de ezequiel aca y en el .h!!!!!!!!!!!!!!
}

bool ControladorRobot11::poseePieza(int id_pieza) {
    sprintf(this->buffer, "Robot 11-%u - poseePieza:", this->id_Robot + 1);
    Logger::setProcessInformation(this->buffer);
    try {
        BufferCanastos canastos;
        while (true) {
            
            Logger::logMessage(Logger::TRACE, "voy a tomar la memoria de los canastos");
            this->semMemCanastos.wait(this->id_semMemCanastos);
            Logger::logMessage(Logger::TRACE, "tomo la memoria de los canastos");

            this->shMemBufferCanastos.readInfo(&canastos);
            this->buscarPosicionPieza(canastos, id_pieza);
            
            sprintf(this->buffer, "Robot 11-%u - poseePieza:", this->id_Robot + 1);
            Logger::setProcessInformation(this->buffer);

            if (this->id_Robot == 0)
                canastos.robotCinta1EsperaPorElLugarNumero = this->posicionPieza;
            else
                canastos.robotCinta2EsperaPorElLugarNumero = this->posicionPieza;

            if (this->posicionPieza != -1) {
                sprintf(this->buffer, "esta el canasto presente en la posicion %d", this->posicionPieza);
                Logger::logMessage(Logger::TRACE, this->buffer);
                if (canastos.canastos[this->posicionPieza].cantidadPiezas != 0)
                    return true;
                
                Logger::logMessage(Logger::TRACE, "no hay piezas en el canasto, aviso que posicion estare esperando y devuelvo mem canastos");
                // me quede sin piezas en el canasto, aviso por que posicion voy a estar esperando
                this->shMemBufferCanastos.writeInfo(&canastos);
                this->semMemCanastos.signal(this->id_semMemCanastos);
                return false;  
            } else {
                Logger::logMessage(Logger::TRACE, "canasto no presente, aviso que posicion espero, devuelvo mem canastos y me duermo");
                this->shMemBufferCanastos.writeInfo(&canastos);
                this->semMemCanastos.signal(this->id_semMemCanastos);

                this->semBloqueoRobot11.wait(this->id_Robot);
            }
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::buscarPosicionPieza(BufferCanastos canastos, int id_pieza) {
    try {
        sprintf(this->buffer, "Robot 11-%u - buscarPosicionPieza:", this->id_Robot + 1);
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