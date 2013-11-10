/* 
 * File:   ControladorCliente.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 12:58 AM
 */

#include "ControladorCliente.h"
#include <Logger.h>

ControladorCliente::ControladorCliente() { }

ControladorCliente::ControladorCliente(long numCliente)
{ 
    sprintf(mensajePantalla, "Cliente #%ld:", numCliente);
    Logger::setProcessInformation(mensajePantalla);
    
    /* Comunicacion con el vendedor */
    this->vendedores = IPC::VendedoresMessageQueue("Vendedor - VendedoresMsgQueue");
    this->vendedores.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
    
    this->clientes = IPC::ClientesMessageQueue("Vendedor - ClientesMsgQueue");
    this->clientes.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    
    this->pedidos = IPC::PedidosVendedorMessageQueue("Vendedor - PedidosMsgQueue");
    this->pedidos.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
    
    this->despacho = IPC::MsgQueue("Cola Cliente Despacho");
    this->despacho.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_OUTPUT_ID);
    
    this->retiro = IPC::MsgQueue("retiro");
    this->retiro.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);
    
    this->numCliente = numCliente;
}

void ControladorCliente::contactarVendedores()
{
    /* Se establece la comunicacion "en dos pasos". */
    mensaje_inicial_t mensaje;
    mensaje.emisor = numCliente;
    mensaje.mtype = CANT_VENDEDORES;
    Logger::logMessage(Logger::TRACE, "Llama a algun vendedor.");
    vendedores.enviarMensajeInicial(mensaje);
    int tiempoRespuesta = (rand() % 20) * 100 * 1000; 
    usleep(tiempoRespuesta);
    
    msg_respuesta_pedido_t mensajeRespuesta;
    clientes.recibirMensajeRespuesta(numCliente, &mensajeRespuesta);
    respuesta_pedido_t respuesta = mensajeRespuesta.respuesta_pedido;
    long numVendedor = respuesta.emisor;
    Logger::logMessage(Logger::TRACE, "Recibe la respuesta del vendedor.");
        
    this->numVendedorAsociado = numVendedor;
}

void ControladorCliente::enviarPedido(int cantidadUnidades, int tipo)
{
    pedido_t pedido;
    pedido.emisor = numCliente;
    pedido.cantidad = cantidadUnidades;
    pedido.tipoProducto = (TipoProducto)tipo;
    pedido.fin = false;
        
    sprintf(mensajePantalla, "Envia al vendedor %ld un "
            "pedido por %d productos de tipo %d.", 
            numVendedorAsociado, pedido.cantidad, pedido.tipoProducto);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    msg_pedido_t mensajePedido;
    mensajePedido.mtype = numVendedorAsociado;
    mensajePedido.pedido = pedido;
    pedidos.enviarMensajePedido(mensajePedido);	    
}

void ControladorCliente::finalizarEnvio(int cantPedidos)
{
    this->cantidadProductos = cantPedidos;
    
    /* Envio el mensaje final. */
    pedido_t pedidoFinal;
    pedidoFinal.emisor = numCliente;
    pedidoFinal.cantidad = 0;
    pedidoFinal.fin = true;
    sprintf(mensajePantalla, "Envia al vendedor %ld el mensaje de "
            "fin de pedido.", numVendedorAsociado);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    msg_pedido_t mensajePedido;
    mensajePedido.mtype = numVendedorAsociado;
    mensajePedido.pedido = pedidoFinal;
    pedidos.enviarMensajePedido(mensajePedido);
}

ControladorCliente::~ControladorCliente() { }

bool ControladorCliente::recibirResultado()
{
    msg_respuesta_pedido_t msgRespuesta;
    clientes.recibirMensajeRespuesta(numCliente, &msgRespuesta);
    return msgRespuesta.respuesta_pedido.recepcionOK;
}

void ControladorCliente::retirarEncargo(TipoProducto & tipoProducto, int & nroOrdenCompra)
{
    PedidoDespacho pedido;
    despacho.recv(TIPO_PEDIDO_DESPACHO, pedido);
    
     sprintf(mensajePantalla, "Se le informa que el Producto %u fue terminado."
             "Procede a ir a la fabrica a buscarlo", tipoProducto);
     Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    tipoProducto = pedido.idProducto_;
    nroOrdenCompra = pedido.idOrdenDeCompra_;
}

Caja ControladorCliente::obtenerProducto(int nroOrdenCompra) {
    EnvioCajaCliente msgCaja;
    retiro.recv(1, msgCaja);
    return msgCaja.caja;
}