/* 
 * File:   ControladorVendedor.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 12:57 AM
 */

#include "ControladorVendedor.h"
#include "LockFile.h"
#include <Logger/Logger.h>

ControladorVendedor::ControladorVendedor() {
}

ControladorVendedor::ControladorVendedor(long numVendedor)
{
    /* Comunicación con los clientes. */
    this->vendedores = IPC::VendedoresMessageQueue("Vendedor - VendedoresMsgQueue");
    this->vendedores.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
    
    this->clientes = IPC::ClientesMessageQueue("Vendedor - ClientesMsgQueue");
    this->clientes.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    
    this->pedidos = IPC::PedidosVendedorMessageQueue("Vendedor - PedidosMsgQueue");
    this->pedidos.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
    
    /* Información sobre las órdenes de producción y compra. */
    this->shmemNumeroOrdenCompra = MemoriaCompartida(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, sizeof(int));
    this->numeroOrdenCompra = (int*) shmemNumeroOrdenCompra.obtener();

    /* Comunicación con el almacén de piezas. */
    this->colaEnvioOrdenProduccion = Cola<mensaje_pedido_fabricacion_t>(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);
    this->colaEnvioOrdenProduccion.obtener();
    
    /* Comunicación con el almacén de productos terminados. */
    this->mutexAlmacenTerminados = IPC::Semaphore("Acceso Almacen Terminados");
    this->mutexAlmacenTerminados.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    
    this->mutexOrdenDeCompra = IPC::Semaphore("mutexOrdenDeCompra");
    this->mutexOrdenDeCompra.getSemaphore(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, 1);
    
    outputQueueDespacho = IPC::MsgQueue("outputQueueDespacho");
    outputQueueDespacho.getMsgQueue(DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_OUTPUT_ID);
    
    this->numVendedor = numVendedor;
}

ControladorVendedor::~ControladorVendedor() { }

long ControladorVendedor::recibirLlamadoTelefonico()
{
    mensaje_inicial_t bufferMensajeInicial;
    this->vendedores.recibirMensajeInicial(CANT_VENDEDORES, &bufferMensajeInicial);

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
	pedidoProduccion.ventaEsValida = false;
	return pedidoProduccion;
    }
    
    if(cantidadEnStock >= pedido.cantidad)
    {
	pedidoProduccion.ventaEsValida = true;
	pedidoProduccion.producidoParaStockear = 0;
	pedidoProduccion.producidoVendido = 0;
	pedidoProduccion.vendidoStockeado = pedido.cantidad;
	pedidoProduccion.diferenciaMinimaProducto = -1;
	return pedidoProduccion;
    }
    
    int cantidadAProducir = pedido.cantidad - cantidadEnStock;
    if(cantidadMinima > cantidadAProducir && cantidadMinima > cantidadEspacioVacio)
    {
	pedidoProduccion.ventaEsValida = false;
	return pedidoProduccion;
    }
    
    cantidadAProducir = std::max(cantidadAProducir, cantidadMinima);
    
    pedidoProduccion.ventaEsValida = true;
    pedidoProduccion.vendidoStockeado = cantidadEnStock;
    pedidoProduccion.producidoVendido = pedido.cantidad - cantidadEnStock;
    pedidoProduccion.producidoParaStockear = cantidadAProducir - pedido.cantidad;
    pedidoProduccion.diferenciaMinimaProducto = cantidadMinima > pedido.cantidad ? cantidadMinima : -1;
    
    return pedidoProduccion;
}

void ControladorVendedor::efectuarReserva(pedido_t pedido, pedido_fabricacion_t pedidoProduccion)
{
    char mensajePantalla[1024];
    sprintf(mensajePantalla, "Se venden %d unidades ya producidas, se mandan a hacer "
    "otras %d de producto para vender y se stockean %d de tipo %d.\n", 
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
    for(int i = 0; i < cantProductos; i++)
    {
	if(pedidoProduccion[i].producidoParaStockear > 0)
	    almacenProductosTerminados.asignarVaciosComoDisponibles(pedidoProduccion[i].producidoParaStockear);

	if(pedidoProduccion[i].producidoVendido > 0)
	    almacenProductosTerminados.asignarVaciosAProduccion(ordenDeCompra, pedidoProduccion[i].producidoVendido);

	if(pedidoProduccion[i].vendidoStockeado > 0)
	    almacenProductosTerminados.asignarStockeados(ordenDeCompra, pedidoProduccion[i].tipoProducto, pedidoProduccion[i].vendidoStockeado);
    }
    
    this->enviarOrdenDeCompraDespacho(ordenDeCompra);
}


void ControladorVendedor::enviarPedidoProduccionAAlmacenPiezas(pedido_fabricacion_t pedidoProduccion)
{    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "Vendedor #%ld envía pedido de producción de %d unidades de producto %d al "
            "almacén de piezas.\n", numVendedor, pedidoProduccion.producidoParaStockear + pedidoProduccion.producidoVendido, 
            pedidoProduccion.tipoProducto);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    mensaje_pedido_fabricacion_t mensaje;
    mensaje.mtype = 1;
    mensaje.pedidoFabricacion = pedidoProduccion;
    colaEnvioOrdenProduccion.enviar(mensaje);
}

void ControladorVendedor::enviarOrdenDeCompraDespacho(OrdenDeCompra ordenDeCompra) {
    // TODO:LOG!!!
    // Primero manda un mensaje avisando que hay una ODC
    PedidoDespacho pedido;
    pedido.idOrdenDeCompra_ = ordenDeCompra.idOrden_;
    pedido.mtype = TIPO_PEDIDO_DESPACHO;
    outputQueueDespacho.send(pedido);
    
    // Luego se manda la ODC con el mtype = idOrden
    PedidoOrdenDeCompra pedidoOrdenDeCompra;
    pedidoOrdenDeCompra.mtype = TIPO_PEDIDO_ODC_DESPACHO;
    pedidoOrdenDeCompra.ordenDeCompra_ = ordenDeCompra;
    outputQueueDespacho.send(pedidoOrdenDeCompra);
}

void ControladorVendedor::enviarConfirmacionDeRecepcionDePedido(long numCliente, respuesta_pedido_t pedido)
{
    msg_respuesta_pedido_t msgRespuesta;
    msgRespuesta.respuesta_pedido = pedido;
    msgRespuesta.mtype = numCliente;
    
    clientes.enviarMensajeRespuesta(msgRespuesta);
}

void ControladorVendedor::confirmarOrdenDeCompraACliente(long numCliente, respuesta_pedido_t pedido)
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
    sprintf(mensajePantalla, "La cantidad mínima de producción de producto %d es %d.\n", numeroProducto, cantMinimaProduccion);
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
    bool ordenDeCompraEsValida = true;
    pedido_fabricacion_t pedidosProduccion[CANT_MAX_PEDIDOS];
    for(int i = 0; i < cantPedidos; i++)
    {
	pedido_fabricacion_t pedidoProduccion = reservarPedido(pedidos[i]);
	pedidosProduccion[i] = pedidoProduccion;
	if(!pedidoProduccion.ventaEsValida)
	{
	    ordenDeCompraEsValida = false;
	    break;
	}

	ordenDeCompra->cantidadPorProducto_[pedidoProduccion.tipoProducto] = pedidos[i].cantidad;

	sprintf(mensajePantalla, "Vendedor #%ld reserva pedido de %d unidades del producto %d.\n", numVendedor, pedidos[i].cantidad, pedidos[i].tipoProducto);
	Logger::getInstance().logMessage(Logger::DEBUG, mensajePantalla);
    }
    
    if(ordenDeCompraEsValida)
	confirmarPedidos(pedidosProduccion, *ordenDeCompra, cantPedidos);
    else
	almacenProductosTerminados.anularReservas();
    
    mutexAlmacenTerminados.signal();
    return ordenDeCompraEsValida;
}