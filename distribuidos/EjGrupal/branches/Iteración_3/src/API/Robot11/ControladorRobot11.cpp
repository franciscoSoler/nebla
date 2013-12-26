/* 
 * File:   ControladorRobot11.cpp
 * Author: francisco
 * 
 * Created on 29 de octubre de 2013, 13:00
 */

#include "ControladorRobot11.h"
#include "../../Logger/Logger.h"
#include "../../Parser/Parser.h"
#include <sstream>
#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControladorRobot11::ControladorRobot11() {
    this->shMem_R11_R14_Data_ = new DataSM_R11_R14();
}

ControladorRobot11::~ControladorRobot11() {
    delete shMem_R11_R14_Data_;
}

void ControladorRobot11::iniciarControlador(int numRobot) {
    try {
        Logger::getInstance();
        sprintf(this->buffer, "Robot 11-%u:", numRobot);
        Logger::setProcessInformation(this->buffer);
        
        this->id_Agv = numRobot * 2 + 1;
        this->nroCinta_ = numRobot + 1;
        this->id_semMemCanastos = numRobot * 2;
        srand (getpid());
        this->id_Robot = numRobot;
        
        
        MiddlewareAPI middleware;
        if ( middleware.crearCanales("Robots11EnBroker", this->nroCinta_, ID_TIPO_ROBOT11) == -1 ) {
            Logger::logMessage(Logger::ERROR, "No se pudieron crear los canales con el Middleware");
            abort();
        }
        
        int idEmisor = this->nroCinta_;
        
        /* Obtengo el semaforo de sincronizacion de Robot11 con AGVs y robot 5*/
        this->semBloqueoRobot5 = COMM::CommSemaphore("semBloqueoRobot5", idEmisor, ID_TIPO_ROBOT11, ID_TIPO_ROBOT5_CINTA);
        this->semBloqueoRobot5.getSemaphore((char*) DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);
        
        this->semMemEstadoRobot5 = COMM::CommSemaphoreMutex<EstadoRobot5>("semMemEstadoRobot5", idEmisor, ID_TIPO_ROBOT11);
        this->semMemEstadoRobot5.getSemaphore((char*) DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);
        this->semMemEstadoRobot5.setShMem(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);

        this->semBloqueoRobot11 = COMM::CommSemaphore("semBloqueoRobot11", idEmisor, ID_TIPO_ROBOT11);
        this->semBloqueoRobot11.getSemaphore((char*) DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, 2);

        this->semBufferCinta6 = COMM::CommSemaphoreMutex<CintaTransportadora_6>("semBufferCinta6", idEmisor, ID_TIPO_ROBOT11);
        this->semBufferCinta6.getSemaphore((char*) DIRECTORY_ROBOT_11, ID_SEM_CINTA_6, 2);

        this->semBufferCanastos = COMM::CommSemaphoreMutex<BufferCanastos>("semMemCanastos", idEmisor, ID_TIPO_ROBOT11);
        this->semBufferCanastos.getSemaphore((char*) DIRECTORY_AGV, ID_SEM_BUFFER_CANASTOS, 3);

        this->semMutex_shMem_R11_R14_ = COMM::CommSemaphoreMutex<SerializedData> ("semMutex_shMem_R11_R14", idEmisor, ID_TIPO_ROBOT11);
        this->semMutex_shMem_R11_R14_.getSemaphore(DIRECTORY_ROBOT_11, SEM_MUTEX_SM_R11_R14_ID, 1);
        
        this->semR11_Cinta13_ = COMM::CommSemaphore ("semR11_Cinta13", idEmisor, ID_TIPO_ROBOT11);
        this->semR11_Cinta13_.getSemaphore(DIRECTORY_ROBOT_11, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
        
        this->semR14_Cinta13_ = COMM::CommSemaphore("semR14_Cinta13", idEmisor, ID_TIPO_ROBOT11, ID_TIPO_ROBOT14);
        this->semR14_Cinta13_.getSemaphore(DIRECTORY_ROBOT_14, SEM_R14_CINTA13_ID, 1);
        
        this->colaPedidosCanastos = IPC::PedidosCanastosMessageQueue("colaPedidosCanastos", idEmisor, ID_TIPO_ROBOT11, ID_TIPO_AGV);
        this->colaPedidosCanastos.getMessageQueue((char*) DIRECTORY_AGV, ID_COLA_PEDIDOS_ROBOTS_AGV);
        
        this->shMemEstadoRobot5 = IPC::EstadoRobot5SharedMemory("shMemEstadoRobot5");
        this->shMemEstadoRobot5.getSharedMemory((char*) DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);

        this->shMem_R11_R14_ = IPC::SharedMemory<SerializedData>("ShMem_R11_R14");
        this->shMem_R11_R14_.getSharedMemory(DIRECTORY_ROBOT_11, SM_R11_R14_ID);        
        this->semMutex_shMem_R11_R14_.setShMem(DIRECTORY_ROBOT_11, SM_R11_R14_ID);
        
        this->shMemBufferCanastos = IPC::BufferCanastosSharedMemory("shMemBufferCanastos");
        
        this->cola11_A_12 = IPC::Barrera1112MessageQueue("cola11_A_12", idEmisor, ID_TIPO_ROBOT11, ID_TIPO_ROBOT12);
        this->cola11_A_12.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_11_A_12);
        this->cola12_A_11 = IPC::Barrera1112MessageQueue("cola12_A_11", idEmisor, ID_TIPO_ROBOT11, ID_TIPO_ROBOT12);
        this->cola12_A_11.getMessageQueue(DIRECTORY_ROBOT_12, ID_COLA_12_A_11);
        
        if (numRobot == 0) {
            this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_0);
            this->semBufferCanastos.setShMem(DIRECTORY_AGV, ID_BUFFER_CANASTOS_0, 0);
            this->shMemBufferCinta6 = IPC::Cinta6SharedMemory("shMemBufferCinta6_0");
            this->shMemBufferCinta6.getSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_0);
            this->semBufferCinta6.setShMem(DIRECTORY_ROBOT_11, ID_CINTA_6_0, 0);
        } else {
            this->shMemBufferCanastos.getSharedMemory((char*) DIRECTORY_AGV, ID_BUFFER_CANASTOS_2);
            this->semBufferCanastos.setShMem(DIRECTORY_AGV, ID_BUFFER_CANASTOS_2, 2);
            this->shMemBufferCinta6 = IPC::Cinta6SharedMemory("shMemBufferCinta6_1");
            this->shMemBufferCinta6.getSharedMemory(DIRECTORY_ROBOT_11, ID_CINTA_6_1);
            this->semBufferCinta6.setShMem(DIRECTORY_ROBOT_11, ID_CINTA_6_1, 1);
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

bool ControladorRobot11::buscarProximoGabinete(EspecifProd *piezas) {
    try {
        sprintf(this->buffer, "Robot 11-%u - buscarProximoGabinete:", this->id_Robot);
        Logger::setProcessInformation(this->buffer);
        
        CintaTransportadora_6 ctrlCinta;
        MensajeBarrera messageBarrera;

        while (true) {
            this->semBufferCinta6.wait(this->id_Robot);
            
            shMemBufferCinta6.readInfo(&ctrlCinta);

            if (ctrlCinta.cantLibres == BUFF_SIZE_CINTA_6) {
                Logger::logMessage(Logger::TRACE, "cinta vacia, aviso que me duermo");
                ctrlCinta.robot11Durmiendo = true;
                this->shMemBufferCinta6.writeInfo(&ctrlCinta);
                this->semBufferCinta6.signal(this->id_Robot);
                
                this->semBloqueoRobot11.wait(this->id_Robot);
            } else {
                this->semBufferCinta6.signal(this->id_Robot);
                break;
            }
        }
        
        // sprintf(this->buffer, "cinta no vacia, veo si hay gabinete para trabajar en el punto de lectura = %d", ctrlCinta.puntoLectura);
        //Logger::logMessage(Logger::TRACE, this->buffer);

        if(!ctrlCinta.lugarVacio[ctrlCinta.puntoLectura]) {
            Logger::logMessage(Logger::TRACE, "hay gabinete, despierto robot 12");
            this->cola11_A_12.send(this->nroCinta_, messageBarrera);
            
            ControladorRobot11::obtenerPantallaDelProducto(ctrlCinta.productoProduccion[ctrlCinta.puntoLectura].tipoProducto, piezas);
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
        sprintf(this->buffer, "Robot 11-%u - avanzarCinta:", this->id_Robot);
        Logger::setProcessInformation(this->buffer);
        CintaTransportadora_6 ctrlCinta;
        EstadoRobot5 estadoRobot5;
        
        this->semBufferCinta6.wait(this->id_Robot);
        // Logger::logMessage(Logger::TRACE, "tome memoria cinta 6");
        
        shMemBufferCinta6.readInfo(&ctrlCinta);    

        ctrlCinta.puntoLectura = (ctrlCinta.puntoLectura + 1) % BUFF_SIZE_CINTA_6;
        imprimirCinta(ctrlCinta.lugarVacio, ctrlCinta.puntoLectura);

        this->shMemBufferCinta6.writeInfo(&ctrlCinta);
        this->semBufferCinta6.signal(this->id_Robot);

        this->semMemEstadoRobot5.wait();
        // Logger::logMessage(Logger::TRACE, "tome memoria estado del robot 5");
        
        this->shMemEstadoRobot5.readInfo(&estadoRobot5);
        if (estadoRobot5.robot5Bloqueado) {
            Logger::logMessage(Logger::TRACE, "robot 5 durmiendo, lo despierto");
            estadoRobot5.robot5Bloqueado = false;
            this->shMemEstadoRobot5.writeInfo(&estadoRobot5);
            this->semBloqueoRobot5.signal(1);
        }
        this->semMemEstadoRobot5.signal();
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

BufferCanastos ControladorRobot11::obtenerBufferCanastos() {
    try {
        BufferCanastos canastos;
        this->semBufferCanastos.wait(this->id_semMemCanastos);
        // Logger::logMessage(Logger::TRACE, "tome la memoria de los canastos");

        this->shMemBufferCanastos.readInfo(&canastos);
        return canastos;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::posicionarCanasto(BufferCanastos canastos) {
    try {
        this->shMemBufferCanastos.writeInfo(&canastos);
        this->semBufferCanastos.signal(this->id_semMemCanastos);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::pedirPiezaFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza) {
    try {
        if (id_Robot == 0)
            canastos.robotCinta1EsperaPorElLugarNumero = posicionPieza;
        else
            canastos.robotCinta2EsperaPorElLugarNumero = posicionPieza;

        this->shMemBufferCanastos.writeInfo(&canastos);

        this->semBufferCanastos.signal(this->id_semMemCanastos);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::pedirCanastoFaltante(int id_Robot, BufferCanastos canastos, int posicionPieza) {
    try {
        if (id_Robot == 0)
            canastos.robotCinta1EsperaPorElLugarNumero = posicionPieza;
        else
            canastos.robotCinta2EsperaPorElLugarNumero = posicionPieza;
        this->shMemBufferCanastos.writeInfo(&canastos);
        this->semBufferCanastos.signal(this->id_semMemCanastos);

        this->semBloqueoRobot11.wait(id_Robot);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::pedirPiezaAlAGV(TipoPieza tipoPieza, int posicionPieza) {
    try {
        sprintf(this->buffer, "Robot 11-%u - pedzirPiezaAlAGV:", this->id_Robot);
        Logger::setProcessInformation(this->buffer);
        Logger::logMessage(Logger::TRACE, "no posee la pieza, se la pido al agv por la cola");
        
        MensajePedidoRobotCinta_6 pedidoCanasto;
        pedidoCanasto.pedidoCanastoAgv.lugar = posicionPieza;
        pedidoCanasto.pedidoCanastoAgv.tipoPieza = tipoPieza;
        this->colaPedidosCanastos.enviarPedidoCanasto(this->id_Agv, pedidoCanasto);
        this->semBloqueoRobot11.wait(this->id_Robot);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

Caja ControladorRobot11::cerrarYTomarCaja() {
    try {
        sprintf(this->buffer, "Robot 11-%u - cerrarYTomarCaja:", this->id_Robot);
        Logger::setProcessInformation(this->buffer);
        
        MensajeBarrera message;
        message.mtype = 1;
        
        this->cola12_A_11.receive(this->nroCinta_, &message);
        
        CintaTransportadora_6 ctrlCinta;
        Caja unaCaja;
        
        this->semBufferCinta6.wait(this->id_Robot);
        // Logger::logMessage(Logger::TRACE, "tome la memoria de la cinta 6 y armo la caja");
        
        shMemBufferCinta6.readInfo(&ctrlCinta);    

        ctrlCinta.cantLibres++;
        ctrlCinta.lugarVacio[ctrlCinta.puntoLectura] = true;
        this->shMemBufferCinta6.writeInfo(&ctrlCinta);
        
        // Logger::logMessage(Logger::TRACE, "devuelvo mem cinta 6");
        this->semBufferCinta6.signal(this->id_Robot);

        unaCaja.idProducto_ = ctrlCinta.productoProduccion[ctrlCinta.puntoLectura].tipoProducto;
        unaCaja.idOrdenDeCompra_ = ctrlCinta.productoProduccion[ctrlCinta.puntoLectura].nroOrdenCompra;

        sprintf(buffer, "el robot 12 termino, puedo cerrar y tomar la caja, la Orden de compra es: %ld, idProducto: %d", unaCaja.idOrdenDeCompra_, unaCaja.idProducto_);
        Logger::logMessage(Logger::IMPORTANT, buffer);

        if (ctrlCinta.productoProduccion[ctrlCinta.puntoLectura].falla || rand() % 100 > 98) {
            unaCaja.fallado_ = true;
        } else {
            unaCaja.fallado_ = false;
        }
        return unaCaja;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::depositarCaja(Caja unaCaja) {
    sprintf(this->buffer, "Robot 11-%u - depositarCaja:", this->id_Robot);
    Logger::setProcessInformation(this->buffer);

    /*if (unaCaja.fallado_) {
        Logger::logMessage(Logger::TRACE, "deposite una caja, estaba rotaaaa!!!!!!!!!!!!!!!!");
    }
    else {
        Logger::logMessage(Logger::TRACE, "deposite una caja, estaba sanaaaa!!!!!!!!!!!!!!!!");
    }*/

    try {
        bool cajaDepositada = false;
        while (! cajaDepositada) {
          obtener_shMem_R11_R14();

          // Se intenta depositar la caja. Sino se puede, se intenta mover
          // la cinta. En caso negativo, el Robot se bloquea
          // TODO: Candidato para meter un ChainOfResponsability
          if ( estaCintaLlena() ) {
              Logger::logMessage(Logger::TRACE, "Cinta13 llena. Robot se bloquea.");
              bloquearRobot();
              continue;
          }
          else if (shMem_R11_R14_Data_->insertarCajaEnCinta(nroCinta_, unaCaja)) {
              sprintf(buffer, "Caja depositada. Cinta13 N°%u: %s", nroCinta_,
                      shMem_R11_R14_Data_->cintaToString(nroCinta_).c_str());
              Logger::logMessage(Logger::DEBUG, buffer);
              cajaDepositada = true;
          }
          else if (estaRobot14TrabajandoEnEstaCinta()) {
              Logger::logMessage(Logger::TRACE, "Robot14 trabajando en la "
              "cinta. Robot se bloquea");
              bloquearRobot();
              continue;
          }
          else if (shMem_R11_R14_Data_->moverCinta(nroCinta_)) {
              Logger::logMessage(Logger::TRACE, "Primer lugar ocupado. Mueve Cinta13");

              if (! shMem_R11_R14_Data_->insertarCajaEnCinta(nroCinta_, unaCaja) ) {
                  throw Exception("Error en Cinta13. No se pudo colocar una "
                      "caja después de haber avanzado la cinta");
              }

              sprintf(buffer, "Caja depositada. Cinta13 N°%u: %s", nroCinta_,
                      shMem_R11_R14_Data_->cintaToString(nroCinta_).c_str());
              Logger::logMessage(Logger::DEBUG, buffer);
              cajaDepositada = true;
          }
          else {
              Logger::logMessage(Logger::TRACE, "No se pudo mover Cinta13. Robot se bloquea");
              bloquearRobot();
              continue;
          }

          // Si se logró colocar la caja, se despierta al Robot14 si este estaba
          // bloqueado
          if (shMem_R11_R14_Data_->estaRobot14Bloqueado()) {
              shMem_R11_R14_Data_->setEstadoBloqueoRobot14(false);
             Logger::logMessage(Logger::TRACE, "Robot14 despertado para trabajar en cinta");
             semR14_Cinta13_.signal();
          }

          liberar_shMem_R11_R14();
        }
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorRobot11::obtenerPantallaDelProducto(TipoProducto tipoProducto, EspecifProd *piezas) {
    ifstream stream;
    stream.open(NOMBRE_ARCHIVO_PRODUCTOS);
    Parser parser;
    
    int ultimoNumeroProductoLeido = 0;
    do
    {
	if(!parser.obtenerLineaSiguiente(stream))
	    break;
	string ultimoNumeroProductoLeidoString = parser.obtenerProximoValor();
	ultimoNumeroProductoLeido = atoi(ultimoNumeroProductoLeidoString.c_str());
    } while(ultimoNumeroProductoLeido != tipoProducto);
    parser.obtenerProximoValor();
    parser.obtenerProximoValor();
    string cantidadPiezasString = parser.obtenerProximoValor();
    int cantPiezas = atoi(cantidadPiezasString.c_str());
    piezas->cantPiezas = 0;
    for (int i = 0; i < cantPiezas; i++) {
        int id = atoi(parser.obtenerProximoValor().c_str());
        int cantidad = atoi(parser.obtenerProximoValor().c_str());
        if (id >= PANTALLA_1) {
            piezas->tipoPantalla.tipoPieza = static_cast<TipoPieza> (id);
            piezas->tipoPantalla.cantidad = cantidad;
            break;
        }
    }
}

bool ControladorRobot11::estaRobot14TrabajandoEnEstaCinta() const {
    return (nroCinta_ == shMem_R11_R14_Data_->getCintaDeTrabajoRobot14());
}

bool ControladorRobot11::estaCintaLlena() const { 
    return shMem_R11_R14_Data_->getCantidadElementosEnCinta(nroCinta_) == CINTA_13_CAPACITY;
}

void ControladorRobot11::bloquearRobot() {
    shMem_R11_R14_Data_->setEstadoBloqueoRobot11(nroCinta_, true);
    liberar_shMem_R11_R14();        
    
    semR11_Cinta13_.wait(nroCinta_ - 1);
    Logger::logMessage(Logger::TRACE, "Es liberado.");
}

void ControladorRobot11::obtener_shMem_R11_R14() {
    SerializedData buffer;

    semMutex_shMem_R11_R14_.wait();
    // Logger::logMessage(Logger::TRACE, "Accede a memoria compartida R14-R16");
    shMem_R11_R14_.read(&buffer);

    shMem_R11_R14_Data_->deserializeData(buffer);
}

void ControladorRobot11::liberar_shMem_R11_R14() {
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Libero la Cinta13: 1: %s, 2: %s", shMem_R11_R14_Data_->cintaToString(1).c_str(),
            shMem_R11_R14_Data_->cintaToString(2).c_str());
    Logger::logMessage(Logger::DEBUG, buffer);

    SerializedData serializedData;
    serializedData = this->shMem_R11_R14_Data_->serializeData();

    shMem_R11_R14_.write( &serializedData );
    // Logger::logMessage(Logger::TRACE, "Libera memoria compartida R14-R16");
    semMutex_shMem_R11_R14_.signal();
}



void ControladorRobot11::imprimirCinta(bool lugarVacio[BUFF_SIZE_CINTA_6], int puntoFinal) {
    std::stringstream ss;
    int inicio = (puntoFinal + BUFF_SIZE_CINTA_6 - 1) % BUFF_SIZE_CINTA_6;

    ss << "Cinta N°" << nroCinta_ << ": |";
    for (int i = BUFF_SIZE_CINTA_6; i > 0; i--) {
        ss << ( (lugarVacio[(inicio + i) % BUFF_SIZE_CINTA_6]) == false ? "x|" : "o|");
    }

    Logger::logMessage(Logger::DEBUG, ss.str());
}
