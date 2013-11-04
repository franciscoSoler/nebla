#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "Common.h"
#include "ControladorVendedor.h"

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
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    
    while(true)
    {
	sprintf(mensajePantalla, "Vendedor #%ld espera clientes.\n", numVendedor);
	write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
	
	mensaje_inicial_t mensajeInicial = controlador.recibirLlamadoTelefonico();
	
	sprintf(mensajePantalla, "Vendedor #%ld recibe mensaje del cliente %ld y establece una comunicación.\n", numVendedor, mensajeInicial.emisor);
	write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
	
	OrdenDeCompra ordenDeCompra = obtenerNuevaOrdenDeCompra(controlador.obtenerNumeroDeOrdenDeCompra(), numVendedor);
	ordenDeCompra.idCliente_ = mensajeInicial.emisor;
	long numCliente = ordenDeCompra.idCliente_;
	
	pedido_t pedido;
	bool pedidoEsValido = true;
	pedido_produccion_t pedidosProduccion[CANT_MAX_PEDIDOS];
	int cantPedidos = 0;
	
	do
	{
	    pedido = controlador.recibirPedido();
	    pedidosProduccion[cantPedidos] = controlador.reservarPedido(pedido);
	    if(!pedidosProduccion[cantPedidos].ventaEsValida)
	    {
		controlador.enviarRespuestaDePedido(numCliente, pedidosProduccion[cantPedidos].ventaEsValida);
		pedidoEsValido = false;
		continue;
	    }
		
	    if(pedido.fin)
		continue;
	    
	    ordenDeCompra.cantidadPorProducto_[pedido.tipoProducto] = pedido.cantidad;
	    
	    sprintf(mensajePantalla, "Vendedor #%ld recibe pedido del cliente %ld de %d unidades del producto %d.\n", numVendedor, pedido.emisor, pedido.cantidad, pedido.tipoProducto);
	    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
	    cantPedidos++;
            
            controlador.enviarRespuestaDePedido(numCliente, pedidoEsValido);
            
	} while(!pedido.fin && pedidoEsValido);
	
	if(pedidoEsValido)
	{
	    for(int i = 0; i < cantPedidos + 1; i++)
            {
		if(pedidosProduccion[i].producidoParaStockear + pedidosProduccion[i].producidoVendido > 0)
		    controlador.enviarPedidoProduccionAAlmacenPiezas(pedidosProduccion[i]);
		controlador.confirmarPedido(pedidosProduccion[i], ordenDeCompra);
	    }	

	}

	else
	{
	    controlador.anularPedidos();
	}
	
	controlador.terminarLlamadoTelefonico();

    }  
    
}

