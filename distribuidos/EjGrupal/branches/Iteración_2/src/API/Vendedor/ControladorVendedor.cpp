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

#include "../../Numerador/numerador.h"
#include "../../Numerador/commonNumerador.h"
#include "SharedMemory.h"

#include <Comunicaciones/Objects/MiddlewareAPI.h>

ControladorVendedor::ControladorVendedor(long numVendedor)
{
    try {
        /* Comunicacion con los clientes. */
        char buffer[255];
        sprintf(buffer, "Vendedor N#%ld:", numVendedor);
        Logger::setProcessInformation(buffer);
        
        MiddlewareAPI middleware;
        middleware.crearCanales(numVendedor, ID_TIPO_VENDEDOR);
         
        this->numVendedor = numVendedor;

        this->vendedores = IPC::VendedorLibreMessageQueue("Vendedor - VendedoresMsgQueue", numVendedor, ID_TIPO_VENDEDOR);
        this->vendedores.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);

        this->clientes = IPC::ClientesMessageQueue("Vendedor - ClientesMsgQueue", numVendedor, ID_TIPO_VENDEDOR, ID_TIPO_CLIENTE);
        this->clientes.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);

        this->pedidos = IPC::PedidosVendedorMessageQueue("Vendedor - PedidosMsgQueue", numVendedor, ID_TIPO_VENDEDOR, ID_TIPO_CLIENTE);
        this->pedidos.getMessageQueue(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);

        /* Informacion sobre las ordenes de produccion y compra. */
        this->shmemNumeroOrdenCompra = IPC::SharedMemory<int>("ultimaOrdenCompra");
        shmemNumeroOrdenCompra.getSharedMemory(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC);
        
        this->shmemAlmacenTerminados = IPC::SharedMemory<AlmacenProductosTerminados>("shMemAlmacenTerminados");
        shmemAlmacenTerminados.getSharedMemory(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS);

        /* Comunicacion con el almacen de piezas. */
        this->colaEnvioOrdenProduccion = COMM::CommMsgHandler(numVendedor, ID_TIPO_VENDEDOR, ID_TIPO_AP);
        this->colaEnvioOrdenProduccion.setReceptorInfo("Vendedor - colaEvioOrdenProduccion",
                                                       DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);

        /* Comunicacion con el almacen de productos terminados. */
        this->mutexAlmacenTerminados = COMM::CommSemaphoreMutex("Acceso Almacen Terminados");
        this->mutexAlmacenTerminados.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);

        this->mutexOrdenDeCompra = COMM::CommSemaphoreMutex("mutexOrdenDeCompra");
        this->mutexOrdenDeCompra.getSemaphore(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, 1);

        inputQueueDespacho = COMM::CommMsgHandler(numVendedor, ID_TIPO_VENDEDOR, ID_TIPO_DESPACHO);
        inputQueueDespacho.setReceptorInfo("inputQueueDespacho", DIRECTORY_DESPACHO, MSGQUEUE_DESPACHO_INPUT_ID);

    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

ControladorVendedor::~ControladorVendedor() {
}

long ControladorVendedor::obtenerNumeroVendedor() {

    retornoVendedor *result_1;
    char *obteneridvendedor_1_arg;

    CLIENT *clnt = clnt_create(HOST, NUMERADORVENDEDOR, NUMERADORVENDEDOR1, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(HOST);
        exit(1);
    }

    // result_1 = obteneridvendedor_1((void*) &obteneridvendedor_1_arg, clnt);
    if (result_1 == (retornoVendedor *) NULL) {
        clnt_perror(clnt, "call failed");
    }

    if ((*result_1).cod_ret != 1) {
        printf("Error mientras se numeraba el vendedor");
    }

    clnt_destroy(clnt);

    this->numVendedor = (*result_1).retornoVendedor_u.idVendedor;

    char buffer[255];
    sprintf(buffer, "Vendedor N#%ld:", this->numVendedor);
    Logger::setProcessInformation(buffer);

    MiddlewareAPI middleware;
    middleware.crearCanales(this->numVendedor, ID_TIPO_VENDEDOR);
    
    return this->numVendedor;
}

void ControladorVendedor::vendedorLibre() {
  /*  void *result_2;
    int vendedorlibre_1_arg = this->numVendedor;

    CLIENT *clnt = clnt_create(HOST, NUMERADORVENDEDOR, NUMERADORVENDEDOR1, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(HOST);
        exit(1);
    }

    result_2 = vendedorlibre_1(&vendedorlibre_1_arg, clnt);
    if (result_2 == (void *) NULL) {
        clnt_perror(clnt, "call failed");
    }*/
}

/*void ControladorVendedor::recibirLlamadoTelefonico()
{
    try {
        mensaje_inicial_t bufferMensajeInicial;
        bufferMensajeInicial.emisor = numVendedor;

        this->vendedores.enviarMensajeInicial( TIPO_VENDEDOR_LIBRE, bufferMensajeInicial );
    }
    catch ( Exception & e ) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}*/

int ControladorVendedor::obtenerNumeroDeOrdenDeCompra()
{
    try {
        int idOrdenDeCompra;
        mutexOrdenDeCompra.wait();
        this->shmemNumeroOrdenCompra.read(&idOrdenDeCompra);
        idOrdenDeCompra++;
        this->shmemNumeroOrdenCompra.write(&idOrdenDeCompra);
        mutexOrdenDeCompra.signal();
        return idOrdenDeCompra;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

pedido_t ControladorVendedor::recibirPedido()
{
    try {
        msg_pedido_t msgPedido;
        pedidos.recibirMensajePedido(numVendedor, &msgPedido);

        char buffer[255];
        sprintf(buffer, "MsgPedidoT: %d - %d", msgPedido.tipo, msgPedido.pedido.tipoProducto);
        Logger::logMessage(Logger::IMPORTANT, buffer);

                return msgPedido.pedido;
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorVendedor::enviarConfirmacionDeRecepcionDePedido(long numCliente, respuesta_pedido_t pedido)
{
    try {
        msg_respuesta_pedido_t msgRespuesta;
        msgRespuesta.respuesta_pedido = pedido;
        msgRespuesta.tipo = 0;
        clientes.enviarMensajeRespuesta(numCliente, msgRespuesta);
        
        char buffer[TAM_BUFFER];
        sprintf(buffer, "Envio confirmacion de recepcion: a cliente: %ld con tipo %d", numCliente, msgRespuesta.tipo);
        Logger::getInstance().logMessage(Logger::TRACE, buffer);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorVendedor::enviarUltimaConfirmacionDeRecepcionDePedido(
        long numCliente, respuesta_pedido_t pedido) {
    try {
        msg_respuesta_pedido_t msgRespuesta;
        msgRespuesta.respuesta_pedido = pedido;
        msgRespuesta.tipo = 1;
        clientes.enviarMensajeRespuesta(numCliente, msgRespuesta);
        
        char buffer[TAM_BUFFER];
        sprintf(buffer, "Envio pedido aceptado: a cliente: %ld con tipo %d", numCliente, msgRespuesta.tipo);
        Logger::getInstance().logMessage(Logger::TRACE, buffer);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

void ControladorVendedor::cancelarOrdenDeCompraACliente(long numCliente, respuesta_pedido_t pedido)
{
    try {
        msg_respuesta_pedido_t msgRespuesta;
        msgRespuesta.respuesta_pedido = pedido;
        msgRespuesta.tipo = 1;
        clientes.enviarMensajeRespuesta(numCliente, msgRespuesta);
        
        char buffer[TAM_BUFFER];
        sprintf(buffer, "Envio pedido cancelado: a cliente: %ld con tipo %d", numCliente, msgRespuesta.tipo);
        Logger::getInstance().logMessage(Logger::TRACE, buffer);
    }
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
}

pedido_fabricacion_t ControladorVendedor::calcularCantidadAProducir(pedido_t pedido, bool* pedidoEnProduccion)
{
    try {
        pedido_fabricacion_t pedidoProduccion;
        pedidoProduccion.tipoProducto = pedido.tipoProducto;

        int cantidadEspacioVacio = almacenProductosTerminados.obtenerEspaciosVacios();
        int cantidadEnStock = almacenProductosTerminados.obtenerCantidadDeStockDeProducto(pedido.tipoProducto);
        int cantidadMinima = obtenerCantidadMinimaDeProduccion(pedido.tipoProducto);

        if(cantidadEspacioVacio + cantidadEnStock < pedido.cantidad)
        {
            /* El pedido no entra en el APT */
            pedidoProduccion.ventaEsValida = false;
            return pedidoProduccion;
        }

        if(cantidadEnStock >= pedido.cantidad)
        {
            /* Pedido es satisfecho con productos stockeados */
            *pedidoEnProduccion = true;
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
            /* Pedido no entra en APT. Es rechazado */
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
    catch (Exception & e) {
        Logger::logMessage(Logger::ERROR, e.get_error_description());
        abort();
    }
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

pedido_fabricacion_t ControladorVendedor::reservarPedido(pedido_t pedido, bool* pedidoEnStock)
{
    bool pedidoEnProduccion = false;
    pedido_fabricacion_t pedidoProduccion = calcularCantidadAProducir(pedido, &pedidoEnProduccion);
    if(pedidoProduccion.ventaEsValida) {
        efectuarReserva(pedido, pedidoProduccion);
        if (pedidoEnProduccion) {
            *pedidoEnStock = almacenProductosTerminados.chequearPedidoReservadoEnStock(pedido.tipoProducto);
        }
    }
    return pedidoProduccion;
}

void ControladorVendedor::confirmarPedidos(pedido_fabricacion_t pedidoProduccion[], OrdenDeCompra ordenDeCompra, int cantProductos)
{
    char mensajePantalla[1024];
    long idOrdenDeCompra = ordenDeCompra.idOrden_;
    for(int i = 0; i < cantProductos; i++)
    {
        if(pedidoProduccion[i].producidoParaStockear > 0)
            almacenProductosTerminados.asignarVaciosComoDisponibles(pedidoProduccion[i].producidoParaStockear,
                                                                    pedidoProduccion[i].tipoProducto);

        if(pedidoProduccion[i].producidoVendido > 0)
            almacenProductosTerminados.asignarVaciosAProduccion(idOrdenDeCompra,
                                                                pedidoProduccion[i].producidoVendido, pedidoProduccion[i].tipoProducto);

        if(pedidoProduccion[i].vendidoStockeado > 0)
            almacenProductosTerminados.asignarStockeados(idOrdenDeCompra, pedidoProduccion[i].vendidoStockeado,
                                                         pedidoProduccion[i].tipoProducto);

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
    mensaje.pedidoFabricacion = pedidoProduccion;
    colaEnvioOrdenProduccion.send(1, mensaje);
}

void ControladorVendedor::enviarOrdenDeCompraDespacho(OrdenDeCompra ordenDeCompra) {
    // Primero manda un mensaje avisando que hay una ODC

    PedidoDespacho pedido;
    pedido.idOrdenDeCompra_ = ordenDeCompra.idOrden_;
    pedido.tipoPedido_ = PEDIDO_ODC;
    pedido.odc_ = ordenDeCompra;

    Msg_PedidoDespacho mensaje_despacho;
    mensaje_despacho.pedido_ = pedido;

    Logger::logMessage(Logger::TRACE, "Envía mensaje con la Orden de Compra");
    inputQueueDespacho.send( ID_DESPACHO, mensaje_despacho );
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
    AlmacenProductosTerminados almacenPT;
    
    mutexAlmacenTerminados.wait();
    sprintf(mensajePantalla, "Paso el mutex del almacen de terminados, "
            "trabajara con la orden Nº: %ld", ordenDeCompra->idOrden_);
    Logger::getInstance().logMessage(Logger::IMPORTANT, mensajePantalla);
    
    this->shmemAlmacenTerminados.read(&almacenPT);
    this->almacenProductosTerminados.establecerMatriz(almacenPT.almacen);
    
    bool ordenDeCompraEsValida = true;
    pedido_fabricacion_t pedidosProduccion[CANT_MAX_PEDIDOS];

    for(int i = 0; i < cantPedidos; i++)
    {
        bool* pedidoEnStock = &(ordenDeCompra->productoTerminado_[pedidos[i].tipoProducto-1]);

        pedido_fabricacion_t pedidoProduccion = reservarPedido(pedidos[i], pedidoEnStock);

        pedidosProduccion[i] = pedidoProduccion;
        pedidosProduccion[i].numOrdenCompra = ordenDeCompra->idOrden_;

        if ( !pedidoProduccion.ventaEsValida )
        {
            ordenDeCompraEsValida = false;
            break;
        }

        ordenDeCompra->cantidadPorProducto_[pedidoProduccion.tipoProducto-1] = pedidos[i].cantidad;

        sprintf(mensajePantalla, "Manda a producir %d unidades del producto %d.",
        pedidosProduccion[i].producidoParaStockear + pedidosProduccion[i].producidoVendido, pedidos[i].tipoProducto);
        Logger::getInstance().logMessage(Logger::DEBUG, mensajePantalla);
    }
    
    if(ordenDeCompraEsValida)
        confirmarPedidos(pedidosProduccion, *ordenDeCompra, cantPedidos);
    else
        almacenProductosTerminados.anularReservas();
    
    this->almacenProductosTerminados.obtenerMatriz(almacenPT.almacen);
    this->shmemAlmacenTerminados.write(&almacenPT);
    
    mutexAlmacenTerminados.signal();

    return ordenDeCompraEsValida;
}
