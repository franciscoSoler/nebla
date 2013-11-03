/* 
 * File:   ControladorRobot5.cpp
 * Author: nebla
 * 
 * Created on October 22, 2013, 2:46 PM
 */

#include "ControladorRobot5.h"
#include "../../Parser/Parser.h"

ControladorRobot5::ControladorRobot5() {
}

ControladorRobot5::~ControladorRobot5() {
}

void ControladorRobot5::iniciarControlador() {
    /* Obtengo la cola de pedidos */
    comunicacionMsgQueue = IPC::ComunicacionRobot5MessageQueue("ComunicacionRobot5MessageQueue");
    comunicacionMsgQueue.getMessageQueue(DIRECTORY_ROBOT_5,ID_COLA_API_ROBOT_5);
}

PedidoRobot5 ControladorRobot5::obtenerPedido() {
    MensajePedidoRobot5 mensajePedido;
    comunicacionMsgQueue.recibirPedidoRobot5(TIPO_PEDIDO_ROBOT_5, &mensajePedido);
    return mensajePedido.pedidoRobot5;
}

void ControladorRobot5::resolverPedido(Canasto canasto, int idDestino) {
    MensajeRespuestaCanasto mensajeRespuesta;
    mensajeRespuesta.idAgv = idDestino;
    mensajeRespuesta.canasto = canasto;
    mensajeRespuesta.mtype = TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5;
    comunicacionMsgQueue.enviarCanasto(mensajeRespuesta);
}

void ControladorRobot5::resolverPedido(ProductoEnProduccion prodEnProduccion, bool ultimo) {
    
    MensajeRespuestaGabinete mensajeRespuesta;
    mensajeRespuesta.mtype = TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5;
    mensajeRespuesta.ultimo = ultimo;
    mensajeRespuesta.productoEnProduccion = prodEnProduccion;
    comunicacionMsgQueue.enviarGabinete(mensajeRespuesta);
}

Canasto ControladorRobot5::obtenerCanasto(TipoProducto tipoPieza) {
    Canasto canasto;
    canasto.tipoPieza = tipoPieza;
    canasto.cantidadPiezas = generateRandomNumber(0, MAX_PIEZAS_CANASTO);
    return canasto;
}

Gabinete ControladorRobot5::obtenerGabinete(TipoProducto tipoPorudcto) {
    Gabinete gabinete;
    gabinete.tipoGabinete = obtenerTipoGabinete(tipoPorudcto);
    return gabinete;
}

int ControladorRobot5::generateRandomNumber(int minValue, int maxValue) {
    srand( time(NULL) + getpid() );
    return minValue + rand() % ( (abs(maxValue - minValue)) );
}

TipoGabinete ControladorRobot5::obtenerTipoGabinete(TipoProducto tipoProducto) {
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