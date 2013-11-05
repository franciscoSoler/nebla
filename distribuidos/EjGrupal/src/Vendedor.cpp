#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "Common.h"
#include "ControladorVendedor.h"
#include "Logger.h"

OrdenDeCompra obtenerNuevaOrdenDeCompra(int numOrdenCompra, int numVendedor)
{
    OrdenDeCompra ordenDeCompra;
    for(int i = 0; i < CANT_PRODUCTOS; i++) {
        ordenDeCompra.cantidadPorProducto_[i] = 0;
        ordenDeCompra.idCliente_ = 0;
        ordenDeCompra.idOrden_ = numOrdenCompra;
        ordenDeCompra.idVendedor_ = numVendedor;
    }

    return ordenDeCompra;
}

int main(int argc, char** argv)
{
    long numVendedor = atoi(argv[1]);
    ControladorVendedor controlador(numVendedor);
    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "Se inicia el vendedor #%ld.\n", numVendedor);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    while(true)
    {
	sprintf(mensajePantalla, "Vendedor #%ld espera clientes.\n", numVendedor);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
	
	long numCliente = controlador.recibirLlamadoTelefonico();
	
	sprintf(mensajePantalla, "Vendedor #%ld recibe mensaje del cliente %ld y establece una comunicación.\n", numVendedor, numCliente);
	Logger::logMessage(Logger::TRACE, mensajePantalla);
	
	OrdenDeCompra ordenDeCompra = obtenerNuevaOrdenDeCompra(controlador.obtenerNumeroDeOrdenDeCompra(), numVendedor);
	ordenDeCompra.idCliente_ = numCliente;
	
	pedido_t pedido;
	pedido_t pedidos[CANT_MAX_PEDIDOS];
	int cantPedidos = 0;
	do
	{
	    pedido = controlador.recibirPedido();
	    pedidos[cantPedidos] = pedido;
	    respuesta_pedido_t respuesta;
	    respuesta.recepcionOK = true;
	    controlador.enviarConfirmacionDeRecepcionDePedido(numCliente, respuesta);
	    if(pedido.fin == true)
		continue;
	    cantPedidos++;
	} while(pedido.fin == false);
	
	bool pedidoEsValido = controlador.realizarOrdenDeCompra(pedidos, &ordenDeCompra, cantPedidos);
	if(pedidoEsValido)
	{
	    respuesta_pedido_t respuesta;
	    respuesta.recepcionOK = true;
	    controlador.confirmarOrdenDeCompraACliente(numCliente, respuesta);
	}
	else
	{
	    respuesta_pedido_t respuesta;
	    respuesta.recepcionOK = false;
	    controlador.cancelarOrdenDeCompraACliente(numCliente, respuesta);
	}

    }  
    
}

