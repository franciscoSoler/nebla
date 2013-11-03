/* 
 * File:   ControladorVendedor.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 12:57 AM
 */

#include "ControladorVendedor.h"

ControladorVendedor::ControladorVendedor() {
}

ControladorVendedor::ControladorVendedor(long numVendedor)
{
    /* Comunicación con los clientes. */
    this->vendedores = Cola<mensaje_inicial_t>(DIRECTORY_VENDEDOR, ID_COLA_VENDEDORES);
    this->vendedores.obtener();
    this->clientes = Cola<respuesta_pedido_t>(DIRECTORY_VENDEDOR, ID_COLA_CLIENTES);
    this->clientes.obtener();
    this->pedidos = Cola<pedido_t>(DIRECTORY_VENDEDOR, ID_COLA_PEDIDOS);
    this->pedidos.obtener();
    
    /* Información sobre las órdenes de producción y compra. */
    this->shmemNumeroOrdenCompra = MemoriaCompartida(DIRECTORY_VENDEDOR, ID_SHMEM_NRO_OC, sizeof(int));
    this->numeroOrdenCompra = (int*) shmemNumeroOrdenCompra.obtener();

    /* Comunicación con el almacén de piezas. */
    this->consultasAlmacen = Cola<consulta_almacen_piezas_t>(DIRECTORY_VENDEDOR, ID_COLA_CONSULTAS_ALMACEN_PIEZAS);
    this->consultasAlmacen.obtener();
    this->respuestasAlmacen = Cola<respuesta_almacen_piezas_t>(DIRECTORY_VENDEDOR, ID_COLA_RESPUESTAS_ALMACEN_PIEZAS);
    this->respuestasAlmacen.obtener();
    
    /* Comunicación con el almacén de productos terminados. */
    this->mutexAlmacenTerminados = IPC::Semaphore("Acceso Almacen Terminados");
    this->mutexAlmacenTerminados.getSemaphore(DIRECTORY_VENDEDOR, ID_ALMACEN_TERMINADOS, 1);
    
    
    this->numVendedor = numVendedor;
}

ControladorVendedor::~ControladorVendedor() { }

mensaje_inicial_t ControladorVendedor::recibirLlamadoTelefonico()
{
    mensaje_inicial_t bufferMensajeInicial;
    vendedores.recibir(0, &bufferMensajeInicial);

    /* Arma el mensaje para contactar al cliente. */
    long numCliente = bufferMensajeInicial.emisor;
    respuesta_pedido_t respuesta;
    respuesta.emisor = numVendedor;
    respuesta.mtype = numCliente;
    clientes.enviar(respuesta);
    
    mutexAlmacenTerminados.wait();        
    return bufferMensajeInicial;
}

int ControladorVendedor::obtenerNumeroDeOrdenDeCompra()
{
    return *numeroOrdenCompra;
}

pedido_t ControladorVendedor::recibirPedido()
{
    pedido_t bufferPedido;
    pedidos.recibir(numVendedor, &bufferPedido);
    return bufferPedido;
}

pedido_produccion_t ControladorVendedor::calcularCantidadAProducir(pedido_t pedido)
{
    pedido_produccion_t pedidoProduccion;
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

void ControladorVendedor::efectuarReserva(pedido_t pedido, pedido_produccion_t pedidoProduccion)
{
    OrdenDeProduccion ordenProduccion;
    if(pedidoProduccion.producidoVendido + pedidoProduccion.producidoParaStockear != 0)
    {
	ordenProduccion.vendedor = numVendedor;
	ordenProduccion.tipoProducto = pedido.tipoProducto;
	
	/*
	consulta_almacen_piezas_t pedidoProduccion;
	pedidoProduccion.tipoConsulta = ORDEN_PRODUCCION;
	pedidoProduccion.cantidad = espacioNecesario;
	pedidoProduccion.cliente = pedido.emisor;
	pedidoProduccion.emisor = numVendedor;
	pedidoProduccion.mtype = 1;
	//consultasAlmacen.enviar(pedidoProduccion);
	 * **/
    }

    char mensajePantalla[1024];
    sprintf(mensajePantalla, "Se venden %d unidades ya producidas, se mandan a hacer otras %d de producto para vender y se stockean %d de tipo %d.\n", pedidoProduccion.vendidoStockeado, pedidoProduccion.producidoVendido , pedidoProduccion.producidoParaStockear, pedido.tipoProducto);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    
    if(pedidoProduccion.producidoParaStockear > 0)
	almacenProductosTerminados.reservarVaciosComoDisponibles(pedidoProduccion.producidoParaStockear);
    
    if(pedidoProduccion.producidoVendido > 0)
	almacenProductosTerminados.reservarVaciosAProduccion(pedidoProduccion.producidoVendido);
    
    if(pedidoProduccion.vendidoStockeado > 0)
	almacenProductosTerminados.reservarStockeados(pedido.tipoProducto, pedidoProduccion.vendidoStockeado);

}

pedido_produccion_t ControladorVendedor::reservarPedido(pedido_t pedido)
{
    
    pedido_produccion_t pedidoProduccion = calcularCantidadAProducir(pedido);
    if(pedidoProduccion.ventaEsValida)
	efectuarReserva(pedido, pedidoProduccion);
    return pedidoProduccion;
}

int ControladorVendedor::obtenerCantidadMinimaDeProduccion(int numProducto)
{
    char mensajePantalla[256];
    
    consulta_almacen_piezas_t consulta;
    consulta.emisor = numVendedor;
    consulta.mtype = 1;
    consulta.tipoProducto = (TipoProducto)numProducto;
    consulta.tipoConsulta = CANT_MINIMA_FABRICACION;
    consultasAlmacen.enviar(consulta);
    sprintf(mensajePantalla, "Vendedor #%ld consulta al almacén cantidad mínima de productos a producir.\n", numVendedor);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));

    respuesta_almacen_piezas_t respuesta;
    respuestasAlmacen.recibir(numVendedor, &respuesta);
    sprintf(mensajePantalla, "Vendedor #%ld recibe que la cantidad mínima de producción del producto %d es de %d.\n", numVendedor, numProducto, respuesta.cantidad);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    
    return respuesta.cantidad;
}

void ControladorVendedor::enviarPedidoProduccionAAlmacenPiezas(pedido_produccion_t pedidoProduccion)
{
    consulta_almacen_piezas_t consulta;
    consulta.emisor = numVendedor;
    consulta.mtype = 1;
    consulta.cantidad = pedidoProduccion.producidoParaStockear + pedidoProduccion.producidoVendido;
    consulta.diferencia = pedidoProduccion.diferenciaMinimaProducto;
    consulta.numOrdenCompra = pedidoProduccion.numOrdenCompra;
    consulta.tipoProducto = pedidoProduccion.tipoProducto;
    consulta.tipoConsulta = ORDEN_PRODUCCION;
    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "Vendedor #%ld envía pedido de producción de %d unidades de producto %d al almacén de piezas.\n", numVendedor, consulta.cantidad, consulta.tipoProducto);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    
    consultasAlmacen.enviar(consulta);
}

void ControladorVendedor::confirmarPedido(pedido_produccion_t pedidoProduccion, OrdenDeCompra ordenDeCompra)
{
    if(pedidoProduccion.producidoParaStockear > 0)
	almacenProductosTerminados.asignarVaciosComoDisponibles(pedidoProduccion.producidoParaStockear);
    
    if(pedidoProduccion.producidoVendido > 0)
	almacenProductosTerminados.asignarVaciosAProduccion(ordenDeCompra, pedidoProduccion.producidoVendido);
    
    if(pedidoProduccion.vendidoStockeado > 0)
	almacenProductosTerminados.asignarStockeados(ordenDeCompra, pedidoProduccion.tipoProducto, pedidoProduccion.vendidoStockeado);
}

void ControladorVendedor::anularPedidos()
{
    almacenProductosTerminados.anularReservas();
}

void ControladorVendedor::terminarLlamadoTelefonico()
{
    mutexAlmacenTerminados.signal();
}

void ControladorVendedor::enviarRespuestaDePedido(long numCliente, bool resultado)
{
    respuesta_pedido_t respuesta;
    respuesta.mtype = numCliente;
    respuesta.recepcionOK = resultado;
    respuesta.emisor = numVendedor;
    clientes.enviar(respuesta);
}