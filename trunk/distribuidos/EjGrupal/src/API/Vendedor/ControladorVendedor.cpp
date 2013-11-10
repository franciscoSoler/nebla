/* 
 * File:   ControladorVendedor.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 12:57 AM
 */

#include "ControladorVendedor.h"
#include "LockFile.h"
#include <Logger/Logger.h>
#include <iostream>

ControladorVendedor::ControladorVendedor(long numVendedor)
{
    /* Comunicacion con los clientes. */
    char buffer[255];
    sprintf(buffer, "Vendedor N#%ld:", numVendedor);
    Logger::setProcessInformation(buffer);
    
    this->vendedores = IPC::VendedoresMessageQueue("Vendedor - VendedoresMsgQueue");
    this->vendedores.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
    
    this->clientes = IPC::ClientesMessageQueue("Vendedor - ClientesMsgQueue");
    this->clientes.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    
    this->pedidos = IPC::PedidosVendedorMessageQueue("Vendedor - PedidosMsgQueue");
    this->pedidos.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
    
    /* Informacion sobre las ordenes de produccion y compra. */
    this->shmemNumeroOrdenCompra = MemoriaCompartida(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, sizeof(int));
    this->numeroOrdenCompra = (int*) shmemNumeroOrdenCompra.obtener();

    /* Comunicacion con el almacen de piezas. */
    this->colaEnvioOrdenProduccion = Cola<mensaje_pedido_fabricacion_t>(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);
    this->colaEnvioOrdenProduccion.obtener();
    
    /* Comunicacion con el almacen de productos terminados. */
    this->mutexAlmacenTerminados = IPC::Semaphore("Acceso Almacen Terminados");
    this->mutexAlmacenTerminados.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    
    this->mutexOrdenDeCompra = IPC::Semaphore("mutexOrdenDeCompra");
    this->mutexOrdenDeCompra.getSemaphore(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, 1);
    
    outputQueueDespacho = IPC::MsgQueue("outputQueueDespacho");
    outputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);
    
    this->numVendedor = numVendedor;
}

ControladorVendedor::~ControladorVendedor() { }

long ControladorVendedor::recibirLlamadoTelefonico()
{
    mensaje_inicial_t bufferMensajeInicial;
    this->vendedores.recibirMensajeInicial(CANT_VENDEDORES, &bufferMensajeInicial);
    int tiempoRespuesta = (rand() % 20) * 100 * 1000; 
    usleep(tiempoRespuesta);

    /* Arma el mensaje para contactar al cliente. */
    long numCliente = bufferMensajeInicial.emisor;

    respuesta_pedido_t respuesta;
    respuesta.emisor = numVendedor;
    
    msg_respuesta_pedido_t mensajeRespuesta;
    mensajeRespuesta.mtype = numCliente;
    mensajeRespuesta.respuesta_pedido = respuesta;

    clientes.enviarMensajeRespuesta(mensajeRespuesta);
    
    return bufferMensajeInicial.emisor;
}

int ControladorVendedor::obtenerNumeroDeOrdenDeCompra()
{
    mutexOrdenDeCompra.wait();
    int idOrdenDeCompra = *numeroOrdenCompra;
    (*numeroOrdenCompra)++;
    mutexOrdenDeCompra.signal();
    return idOrdenDeCompra;
}

pedido_t ControladorVendedor::recibirPedido()
{
    msg_pedido_t msgPedido;
    pedidos.recibirMensajePedido(numVendedor, &msgPedido);
    return msgPedido.pedido;
}

pedido_fabricacion_t ControladorVendedor::calcularCantidadAProducir(pedido_t pedido)
{
    pedido_fabricacion_t pedidoProduccion;
    pedidoProduccion.tipoProducto = pedido.tipoProducto;
    
    int cantidadEspacioVacio = almacenProductosTerminados.obtenerEspaciosVacios();
    int cantidadEnStock = almacenProductosTerminados.obtenerCantidadDeStockDeProducto(pedido.tipoProducto);
    int cantidadMinima = obtenerCantidadMinimaDeProduccion(pedido.tipoProducto);
    
    if(cantidadEspacioVacio + cantidadEnStock < pedido.cantidad)
    {
        /* El pedido no entra en el almacen de productos terminados */
	pedidoProduccion.ventaEsValida = false;
	return pedidoProduccion;
    }
    
    if(cantidadEnStock >= pedido.cantidad)
    {
        /* Me alcanza con lo que ya hay en el almacen de productos terminados */
	pedidoProduccion.ventaEsValida = true;
	pedidoProduccion.producidoParaStockear = 0;
	pedidoProduccion.producidoVendido = 0;
	pedidoProduccion.vendidoStockeado = pedido.cantidad;
	pedidoProduccion.diferenciaMinimaProducto = 0;
	return pedidoProduccion;
    }
    
    int cantidadAProducir = pedido.cantidad - cantidadEnStock;
    if(cantidadMinima > cantidadAProducir && cantidadMinima > cantidadEspacioVacio)
    {
        /* El pedido no entra en el almacen de productos terminados */
	pedidoProduccion.ventaEsValida = false;
	return pedidoProduccion;
    }
    
    cantidadAProducir = std::max(cantidadAProducir, cantidadMinima);
    
    pedidoProduccion.ventaEsValida = true;
    pedidoProduccion.vendidoStockeado = cantidadEnStock;
    pedidoProduccion.producidoVendido = pedido.cantidad - cantidadEnStock; // Cantidad final a producir
    pedidoProduccion.producidoParaStockear = cantidadAProducir - pedidoProduccion.producidoVendido;
    pedidoProduccion.diferenciaMinimaProducto = cantidadMinima < cantidadAProducir ? 0 : cantidadMinima - pedidoProduccion.producidoVendido;
    
    return pedidoProduccion;
}

void ControladorVendedor::efectuarReserva(pedido_t pedido, pedido_fabricacion_t pedidoProduccion)
{
    char mensajePantalla[1024];
    sprintf(mensajePantalla, "Se venden %d unidades ya producidas, se mandan a hacer "
		"otras %d de producto para vender y se stockean %d de tipo %d.", 
    pedidoProduccion.vendidoStockeado, pedidoProduccion.producidoVendido , 
    pedidoProduccion.producidoParaStockear, pedido.tipoProducto);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    if(pedidoProduccion.producidoParaStockear > 0)
	almacenProductosTerminados.reservarVaciosComoDisponibles(pedidoProduccion.producidoParaStockear);
    
    if(pedidoProduccion.producidoVendido > 0)
	almacenProductosTerminados.reservarVaciosAProduccion(pedidoProduccion.producidoVendido);
    
    if(pedidoProduccion.vendidoStockeado > 0)
	almacenProductosTerminados.reservarStockeados(pedido.tipoProducto, pedidoProduccion.vendidoStockeado);

}

pedido_fabricacion_t ControladorVendedor::reservarPedido(pedido_t pedido)
{
    
    pedido_fabricacion_t pedidoProduccion = calcularCantidadAProducir(pedido);
    if(pedidoProduccion.ventaEsValida)
	efectuarReserva(pedido, pedidoProduccion);
    return pedidoProduccion;
}

void ControladorVendedor::confirmarPedidos(pedido_fabricacion_t pedidoProduccion[], OrdenDeCompra ordenDeCompra, int cantProductos)
{
    char mensajePantalla[1024];
    for(int i = 0; i < cantProductos; i++)
    {
	if(pedidoProduccion[i].producidoParaStockear > 0)
	    almacenProductosTerminados.asignarVaciosComoDisponibles(pedidoProduccion[i].producidoParaStockear, pedidoProduccion[i].tipoProducto);

	if(pedidoProduccion[i].producidoVendido > 0)
	    almacenProductosTerminados.asignarVaciosAProduccion(ordenDeCompra, pedidoProduccion[i].producidoVendido, pedidoProduccion[i].tipoProducto);

	if(pedidoProduccion[i].vendidoStockeado > 0)
	    almacenProductosTerminados.asignarStockeados(ordenDeCompra, pedidoProduccion[i].vendidoStockeado, pedidoProduccion[i].tipoProducto);
   
	this->enviarPedidoProduccionAAlmacenPiezas(pedidoProduccion[i]);
    }
    
    sprintf(mensajePantalla, "Envia orden de compra número %ld de cliente %ld a despacho.", 
	    ordenDeCompra.idOrden_, ordenDeCompra.idCliente_);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    this->enviarOrdenDeCompraDespacho(ordenDeCompra);
}


void ControladorVendedor::enviarPedidoProduccionAAlmacenPiezas(pedido_fabricacion_t pedidoProduccion)
{    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "Envia pedido de produccion de %d unidades de producto %d al "
            "almacen de piezas. Dif minima: %d Orden de compra: %d", 
	    pedidoProduccion.producidoParaStockear + pedidoProduccion.producidoVendido, 
            pedidoProduccion.tipoProducto,
    pedidoProduccion.diferenciaMinimaProducto,
    pedidoProduccion.numOrdenCompra);
    
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    mensaje_pedido_fabricacion_t mensaje;
    mensaje.mtype = 1;
    mensaje.pedidoFabricacion = pedidoProduccion;
    colaEnvioOrdenProduccion.enviar(mensaje);
}

void ControladorVendedor::enviarOrdenDeCompraDespacho(OrdenDeCompra ordenDeCompra) {
    // Primero manda un mensaje avisando que hay una ODC

    PedidoDespacho pedido;
    pedido.idOrdenDeCompra_ = ordenDeCompra.idOrden_;

    Msg_PedidoDespacho mensaje_despacho;
    mensaje_despacho.mtype = MSG_PEDIDO_DESPACHO;
    mensaje_despacho.pedido_ = pedido;

    Logger::logMessage(Logger::TRACE, "Envía mensaje avisando de envío de Orden de Compra");
    outputQueueDespacho.send( mensaje_despacho );


    Msg_EnvioODCDespacho mensaje_odc;
    mensaje_odc.mtype = MSG_ENVIO_ODC_DESPACHO;
    mensaje_odc.ordenDeCompra_ = ordenDeCompra;

    Logger::logMessage(Logger::TRACE, "Envía Orden de Compra");
    outputQueueDespacho.send( mensaje_odc );
}

void ControladorVendedor::enviarConfirmacionDeRecepcionDePedido(long numCliente, respuesta_pedido_t pedido)
{
    msg_respuesta_pedido_t msgRespuesta;
    msgRespuesta.respuesta_pedido = pedido;
    msgRespuesta.mtype = numCliente;
    
    clientes.enviarMensajeRespuesta(msgRespuesta);
}

void ControladorVendedor::cancelarOrdenDeCompraACliente(long numCliente, respuesta_pedido_t pedido)
{
    msg_respuesta_pedido_t msgRespuesta;
    msgRespuesta.respuesta_pedido = pedido;
    msgRespuesta.mtype = numCliente;
    
    clientes.enviarMensajeRespuesta(msgRespuesta);
}

int ControladorVendedor::obtenerCantidadMinimaDeProduccion(int numeroProducto)
{
    ifstream stream;
    stream.open(NOMBRE_ARCHIVO_PRODUCTOS);
    Parser parser;
    this->buscarUbicacionDeProductoEnArchivo(parser, stream, numeroProducto);
    string cantMinimaProduccionString = parser.obtenerProximoValor();
    cantMinimaProduccionString = parser.obtenerProximoValor();
    int cantMinimaProduccion = atoi(cantMinimaProduccionString.c_str());
    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "La cantidad minima de produccion de producto %d es %d.", numeroProducto, cantMinimaProduccion);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    return cantMinimaProduccion;
}

void ControladorVendedor::buscarUbicacionDeProductoEnArchivo(Parser parser, ifstream& stream, int numeroProducto)
{
    bool continuaArchivo = true;
    int ultimoNumeroProductoLeido = 0;
    do
    {
	continuaArchivo = parser.obtenerLineaSiguiente(stream);
	if(!continuaArchivo)
	    continue;
	string ultimoNumeroProductoLeidoString = parser.obtenerProximoValor();
	ultimoNumeroProductoLeido = atoi(ultimoNumeroProductoLeidoString.c_str());
    } while(continuaArchivo && ultimoNumeroProductoLeido != numeroProducto);
}

bool ControladorVendedor::realizarOrdenDeCompra(pedido_t pedidos[], OrdenDeCompra* ordenDeCompra, int cantPedidos)
{
    char mensajePantalla[1024];
    mutexAlmacenTerminados.wait();
    Logger::getInstance().logMessage(Logger::IMPORTANT, "Paso el mutex del almacen de terminados");
    bool ordenDeCompraEsValida = true;
    pedido_fabricacion_t pedidosProduccion[CANT_MAX_PEDIDOS];
    for(int i = 0; i < cantPedidos; i++)
    {
	pedido_fabricacion_t pedidoProduccion = reservarPedido(pedidos[i]);
	pedidosProduccion[i] = pedidoProduccion;
        pedidosProduccion[i].numOrdenCompra = ordenDeCompra->idOrden_;
	if(!pedidoProduccion.ventaEsValida)
	{
	    ordenDeCompraEsValida = false;
	    break;
	}

	ordenDeCompra->cantidadPorProducto_[pedidoProduccion.tipoProducto] = pedidos[i].cantidad;

	sprintf(mensajePantalla, "Manda a producir %d unidades del producto %d.", 
		pedidosProduccion[i].producidoParaStockear + pedidosProduccion[i].producidoVendido, pedidos[i].tipoProducto);
	Logger::getInstance().logMessage(Logger::DEBUG, mensajePantalla);
    }
    
    if(ordenDeCompraEsValida)
	confirmarPedidos(pedidosProduccion, *ordenDeCompra, cantPedidos);
    else
	almacenProductosTerminados.anularReservas();
    
    mutexAlmacenTerminados.signal();
    return ordenDeCompraEsValida;
}
