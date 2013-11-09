/* 
 * File:   ControladorRobot5Cinta.cpp
 * Author: knoppix
 * 
 * Created on November 9, 2013, 1:17 PM
 */

#include "ControladorRobot5Cinta.h"

#include "../../Logger/Logger.h"
#include "../../Parser/Parser.h"

ControladorRobot5Cinta::ControladorRobot5Cinta() :
        colaPedidosProduccion("PedidosProduccionMessageQueue"),
        estadoRobot5("EstadoRobot5SharedMemory"),
        semaforoAccesoEstadoRobot5("AccesoEstadoRobot5"),
        semaforoBloqueoRobot5("BloqueoRobot5"),
        semaforoBloqueoRobot11("BloqueoRobot11"),
        semaforoApiRobot5("ApiRobot5")
{
        cintaTransportadora[0] = CintaTransportadora6(0);
        cintaTransportadora[1] = CintaTransportadora6(1);
}

ControladorRobot5Cinta::~ControladorRobot5Cinta() {
}

void ControladorRobot5Cinta::iniciarControlador() { 
    
    /* Obtengo la cola de pedidos */
    colaPedidosProduccion.getMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_PEDIDOS_PRODUCCION);
              
    /* Obtengo las cintas transportadoras */    
    cintaTransportadora[0].iniciarCinta(ID_CINTA_6_0, ID_SEM_CINTA_6);
    cintaTransportadora[1].iniciarCinta(ID_CINTA_6_1, ID_SEM_CINTA_6);

    /* Obtengo la memoria compartida del estado del robot 5 y su semaforo de acceso */
    estadoRobot5.getSharedMemory(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5);
    semaforoAccesoEstadoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_ESTADO_ROBOT_5, 1);
    semaforoBloqueoRobot5.getSemaphore(DIRECTORY_ROBOT_5, ID_SEM_BLOQUEO_ROBOT_5, 1);

    semaforoBloqueoRobot11.getSemaphore(DIRECTORY_ROBOT_11, ID_SEM_BLOQUEO_ROBOT_11, CANTIDAD_CINTAS_6);
    
    /* Obtengo el semaforo para la api del robot 5 */
    semaforoApiRobot5.getSemaphore(DIRECTORY_ROBOT_5,ID_SEM_API_ROBOT_5, 1);
}

PedidoProduccion ControladorRobot5Cinta::obtenerPedidoProduccion() {
    /* Recibo una orden de produccion */
    MensajePedidoProduccion mensajePedidoProduccion;
    colaPedidosProduccion.recibirPedidoProduccion(TIPO_PEDIDO_PRODUCCION, &mensajePedidoProduccion);

    char buffer[TAM_BUFFER];
    sprintf(buffer, "Recibí un pedido de produccion:\n\tOrden de compra: %d Tipo producto: %d Cantidad: %d Diferencia: %d",
                    mensajePedidoProduccion.pedidoProduccion.nroOrdenCompra,
                    mensajePedidoProduccion.pedidoProduccion.tipo,
                    mensajePedidoProduccion.pedidoProduccion.cantidad,
                    mensajePedidoProduccion.pedidoProduccion.diferenciaMinima);

    Logger::getInstance().logMessage(Logger::TRACE, buffer);
    return mensajePedidoProduccion.pedidoProduccion;
}

EstadoCinta ControladorRobot5Cinta::obtenerEstadoCinta(int numeroCinta) {
    return cintaTransportadora[numeroCinta].obtenerEstadoCinta();
}

void ControladorRobot5Cinta::leerEstadoCintas(EstadoCinta &estadoCinta0,EstadoCinta &estadoCinta1) {
    estadoCinta0 = obtenerEstadoCinta(0);
    estadoCinta1 = obtenerEstadoCinta(1);
}
    
void ControladorRobot5Cinta::obtenerPedidoGabinete() {
    
    EstadoCinta estadoCinta0;
    EstadoCinta estadoCinta1;
    
    /* Obtengo los estados de ambas cintas para intentar depositar el primer gbainete */            
    Logger::getInstance().logMessage(Logger::TRACE, "Verificando estado de las cintas.");
    leerEstadoCintas(estadoCinta0, estadoCinta1);
    bool bloqueadas = (estadoCinta0.ocupado && estadoCinta1.ocupado);
    while (bloqueadas) {
        Logger::getInstance().logMessage(Logger::TRACE, "Ambas cintas estan bloqueadas.");
        /* Como ambas cintas tienen su primer lugar ocupado, 
         * aviso que me voy a bloquear y me bloqueo */
        semaforoAccesoEstadoRobot5.wait();
        {
            EstadoRobot5 estadoRobot;
            estadoRobot.robot5Bloqueado = true;
            estadoRobot5.writeInfo(&estadoRobot);
        }
        semaforoAccesoEstadoRobot5.signal();
        semaforoBloqueoRobot5.wait();
        /* Cuando me libero debo verificar que se haya liberado 
         * alguna de las cintas
         */
        leerEstadoCintas(estadoCinta0, estadoCinta1);
        bloqueadas = (estadoCinta0.ocupado && estadoCinta1.ocupado);
    }
    Logger::getInstance().logMessage(Logger::TRACE, "Se libero un lugar en alguna cinta, envio un pedido por un producto.");

    semaforoApiRobot5.wait();
}

void ControladorRobot5Cinta::resolverPedidoGabinete(ProductoEnProduccion productoEnProduccion, int cintaAUtilizar) {

    Logger::getInstance().logMessage(Logger::TRACE, "Deposito el producto en produccion.");
    cintaTransportadora[cintaAUtilizar].depositarProductoEnProduccion(productoEnProduccion);

    char buffer[TAM_BUFFER];
    sprintf(buffer, "Estado cinta %d luego del deposito: %s", cintaAUtilizar, cintaTransportadora[cintaAUtilizar].obtenerMensajeEstado().c_str());
    Logger::getInstance().logMessage(Logger::TRACE, buffer);

    /* Verifico si el robot 11 encargado de la cinta utilizada esta 
     * bloqueado 
     */
    Logger::getInstance().logMessage(Logger::TRACE, "Verifico si el robot 11 correspondiente esta bloqueado.");
    if (cintaTransportadora[cintaAUtilizar].robot11Bloqueado()) {
        cintaTransportadora[cintaAUtilizar].marcarRobot11Liberado();
        semaforoBloqueoRobot11.signal(cintaAUtilizar);
        Logger::getInstance().logMessage(Logger::TRACE, "Robot 11 bloqueado, lo marco como desbloqueado y lo desbloqueo.");
    }
    
    //semaforoApiRobot5.signal();
}

void ControladorRobot5Cinta::avisarProximoPedido() {
    Logger::getInstance().logMessage(Logger::TRACE, "Envio mensaje al almacen de piezas para que envie la proxima orden de produccion.");
    MensajeProximoPedidoProduccion mensajeProximoPedido;
    mensajeProximoPedido.mtype = TIPO_PEDIDO_ROBOT_5_ALMACEN_PIEZAS;
    colaPedidosProduccion.enviarProximoPedidoProduccion(mensajeProximoPedido);

    semaforoApiRobot5.signal();
}

Gabinete ControladorRobot5Cinta::obtenerGabinete(TipoProducto tipoPorudcto) {
    Gabinete gabinete;
    gabinete.tipoGabinete = obtenerTipoGabinete(tipoPorudcto);
    return gabinete;
}
    
TipoGabinete ControladorRobot5Cinta::obtenerTipoGabinete(TipoProducto tipoProducto) {
    std::ifstream stream;
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
    
    return static_cast<TipoGabinete> (atoi(parser.obtenerProximoValor().c_str()));
}

/* Metodos privados, sin implementacion, para evitar copias del objeto */
ControladorRobot5Cinta::ControladorRobot5Cinta(const ControladorRobot5Cinta& orig) {
}

ControladorRobot5Cinta& ControladorRobot5Cinta::operator= (const ControladorRobot5Cinta &p) {
    return *this;
}
