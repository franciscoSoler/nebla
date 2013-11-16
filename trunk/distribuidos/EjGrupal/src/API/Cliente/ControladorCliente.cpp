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
    this->despacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);
    
    this->retiro = IPC::MsgQueue("retiro");
    this->retiro.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);

    this->inputQueueCliente = IPC::MsgQueue("inputQueueCliente");
    this->inputQueueCliente.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);
    
    this->numCliente = numCliente;
}

void ControladorCliente::contactarVendedores()
{
    try {
        /* Se establece la comunicacion "en dos pasos". */
        mensaje_inicial_t mensaje;
        mensaje.emisor = numCliente;
        mensaje.mtype = TIPO_BUSCANDO_VENDEDOR;
        Logger::logMessage(Logger::TRACE, "Llama a algun vendedor.");
        vendedores.enviarMensajeInicial(mensaje);

        msg_respuesta_pedido_t mensajeRespuesta;
        clientes.recibirMensajeRespuesta(numCliente, &mensajeRespuesta);
        respuesta_pedido_t respuesta = mensajeRespuesta.respuesta_pedido;
        long numVendedor = respuesta.emisor;
        Logger::logMessage(Logger::TRACE, "Recibe la respuesta del vendedor.");

        this->numVendedorAsociado = numVendedor;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorCliente::enviarPedido(int cantidadUnidades, int tipo)
{
    try {
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
        mensajePedido.tipo = 0;
        pedidos.enviarMensajePedido(mensajePedido);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
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
    mensajePedido.tipo = 1; // Tipo != 0 fin de la comunicacion
    pedidos.enviarMensajePedido(mensajePedido);
}

ControladorCliente::~ControladorCliente() { }

respuesta_pedido_t ControladorCliente::recibirResultado()
{
    msg_respuesta_pedido_t msgRespuesta;
    clientes.recibirMensajeRespuesta(numCliente, &msgRespuesta);
    return msgRespuesta.respuesta_pedido;
}

void ControladorCliente::esperarPedido(TipoProducto & tipoProducto, int & nroOrdenCompra, int numCliente)
{
    Msg_PedidoDespacho mensaje;
    this->inputQueueCliente.recv(numCliente, mensaje);
    PedidoDespacho pedido = mensaje.pedido_;
    
     sprintf(mensajePantalla, "Se le informa que el Producto %u fue terminado. "
             "Procede a ir a la fabrica a buscarlo", pedido.idProducto_);
     Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    tipoProducto = pedido.idProducto_;
    nroOrdenCompra = pedido.idOrdenDeCompra_;
}

void ControladorCliente::retirarEncargo(TipoProducto tipo, int nroOrden) {
    char buffer[255];
    sprintf(buffer, "Cliente va a retirar el producto N°%d de la ODC N°%d", tipo, nroOrden);
    Logger::logMessage(Logger::TRACE, buffer);

    PedidoDespacho pedido;
    pedido.idOrdenDeCompra_ = nroOrden;
    pedido.idProducto_ = tipo;
    pedido.tipoPedido_ = PEDIDO_CLIENTE;

    Msg_PedidoDespacho mensaje;

    mensaje.mtype = MSG_PEDIDO_DESPACHO;
    mensaje.pedido_ = pedido;

    despacho.send(mensaje);
}

Caja ControladorCliente::obtenerProducto(int idCliente) {
    Msg_EnvioCajaCliente msgCaja;
    retiro.recv(idCliente, msgCaja);
    Logger::logMessage(Logger::IMPORTANT, "Recibe Caja de Robot16");
    return msgCaja.caja;
}
