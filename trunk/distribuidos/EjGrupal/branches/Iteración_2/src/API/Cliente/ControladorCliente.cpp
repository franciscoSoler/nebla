/* 
 * File:   ControladorCliente.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 12:58 AM
 */

#include "ControladorCliente.h"
#include <Logger.h>

#include "../../Numerador/numeradorClientes.h"
#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControladorCliente::ControladorCliente(long numCliente) {
    try {

        sprintf(mensajePantalla, "Cliente N°%ld:", numCliente);
        Logger::setProcessInformation(mensajePantalla);

        this->numCliente = numCliente;

        MiddlewareAPI middleware;
        middleware.crearCanales(numCliente, ID_TIPO_CLIENTE);

        /* Comunicacion con el vendedor */
        /*this->vendedores = IPC::VendedorLibreMessageQueue("Vendedor - VendedorLibreMsgQueue");
        this->vendedores.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);*/

        this->clientes = IPC::ClientesMessageQueue("Vendedor - ClientesMsgQueue", numCliente, ID_TIPO_CLIENTE, ID_TIPO_VENDEDOR);
        this->clientes.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);

        this->pedidos = IPC::PedidosVendedorMessageQueue("Vendedor - PedidosMsgQueue", numCliente, ID_TIPO_VENDEDOR, ID_TIPO_CLIENTE);
        this->pedidos.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);

        this->inputQueueDespacho = IPC::MsgQueue("inputQueueDespacho", numCliente, ID_TIPO_DESPACHO, ID_TIPO_CLIENTE);
        this->inputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);

        this->R16_Cliente_Queue_ = IPC::MsgQueue("R16_Cliente_Queue_", numCliente, ID_TIPO_CLIENTE, ID_TIPO_ROBOT16_DESPACHO);
        this->R16_Cliente_Queue_.getMsgQueue(DIRECTORY_ROBOT_16, MSGQUEUE_R16_CLIENT_ID);

        this->inputQueueCliente = IPC::MsgQueue("inputQueueCliente", numCliente, ID_TIPO_CLIENTE, ID_TIPO_DESPACHO);
        this->inputQueueCliente.getMsgQueue(DIRECTORY_CLIENTE, MSGQUEUE_CLIENT_INPUT_ID);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}


int ControladorCliente::obtenerNumeroCliente() {
	CLIENT *clnt;
	retorno  *result_1;
	char *obteneridcliente_1_arg;

	clnt = clnt_create ("LOCALHOST", NUMERADORCLIENTE, NUMERADORCLIENTE1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror ("LOCALHOST");
		exit (1);
	}

	result_1 = obteneridcliente_1((void*)&obteneridcliente_1_arg, clnt);
	if (result_1 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	clnt_destroy (clnt);

	this->numCliente = (*result_1).retorno_u.numero;
        
        sprintf(mensajePantalla, "Cliente #%ld:",this->numCliente);
        Logger::setProcessInformation(mensajePantalla);
        
        return this->numCliente;
}


void ControladorCliente::contactarVendedores()
{
    try {
        /* Se establece la comunicacion "en dos pasos". */
        /*mensaje_inicial_t mensaje;
        mensaje.emisor = numCliente;
        Logger::logMessage(Logger::TRACE, "Llama a algun vendedor.");
        vendedores.enviarMensajeInicial(TIPO_BUSCANDO_VENDEDOR, mensaje);

        msg_respuesta_pedido_t mensajeRespuesta;
        clientes.recibirMensajeRespuesta(numCliente, &mensajeRespuesta);
        respuesta_pedido_t respuesta = mensajeRespuesta.respuesta_pedido;
        long numVendedor = respuesta.emisor;
        Logger::logMessage(Logger::TRACE, "Recibe la respuesta del vendedor.");*/

        this->numVendedorAsociado = 1;
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
        mensajePedido.pedido = pedido;
        mensajePedido.tipo = 0;
        pedidos.enviarMensajePedido(numVendedorAsociado, mensajePedido);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorCliente::finalizarEnvio(int cantPedidos)
{
    try {
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
        mensajePedido.pedido = pedidoFinal;
        mensajePedido.tipo = 1; // Tipo != 0 fin de la comunicacion
        pedidos.enviarMensajePedido(numVendedorAsociado, mensajePedido);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

ControladorCliente::~ControladorCliente() { }

respuesta_pedido_t ControladorCliente::recibirResultado()
{
    try {
        msg_respuesta_pedido_t msgRespuesta;
        clientes.recibirMensajeRespuesta(numCliente, &msgRespuesta);
        
        char buffer[TAM_BUFFER];
        if ( msgRespuesta.respuesta_pedido.recepcionOK) {
            sprintf(buffer, "Pedido tomado: tipo %d", msgRespuesta.tipo);
        } else {
            sprintf(buffer, "Pedido Rechazado: tipo %d", msgRespuesta.tipo);
        }
        Logger::getInstance().logMessage(Logger::TRACE, buffer);
        
        return msgRespuesta.respuesta_pedido;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorCliente::esperarPedido(TipoProducto & tipoProducto, int & nroOrdenCompra, int numCliente)
{
    try {
        Msg_RetiroProducto mensaje;
        this->inputQueueCliente.recv(numCliente, mensaje);
        PedidoDespacho pedido = mensaje.datos_;

         sprintf(mensajePantalla, "Se le informa que el Producto %u fue terminado. "
                 "Procede a ir a la fabrica a buscarlo", pedido.idProducto_);
         Logger::logMessage(Logger::TRACE, mensajePantalla);

        tipoProducto = pedido.idProducto_;
        nroOrdenCompra = pedido.idOrdenDeCompra_;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorCliente::retirarEncargo(TipoProducto tipo, int nroOrden, bool ultimoPedido) {
    try {
        char buffer[255];
        sprintf(buffer, "Cliente va a retirar el producto N°%d de la ODC N°%d", tipo, nroOrden);
        Logger::logMessage(Logger::TRACE, buffer);

        PedidoDespacho pedido;
        pedido.idOrdenDeCompra_ = nroOrden;
        pedido.idProducto_ = tipo;
        pedido.tipoPedido_ = PEDIDO_CLIENTE;

        Msg_PedidoDespacho mensaje;
        mensaje.pedido_ = pedido;
        mensaje.ultimoPedido_ = ultimoPedido;

        inputQueueDespacho.send(MSG_PEDIDO_DESPACHO, mensaje);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

Caja ControladorCliente::obtenerProducto(int idCliente) {
    try {
        Msg_EnvioCajaCliente msgCaja;
        R16_Cliente_Queue_.recv(idCliente, msgCaja);
        Logger::logMessage(Logger::IMPORTANT, "Recibe Caja de Robot16");
        return msgCaja.caja;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}
