#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "const.h"
#include "ControladorVendedor.h"

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
	
	pedido_t pedido;
	do
	{
	    pedido = controlador.recibirPedido();
	    
	    if(pedido.tipoProducto == FIN_ENVIO)
		continue;
	    
	    sprintf(mensajePantalla, "Vendedor #%ld recibe pedido del cliente %ld de %d unidades del producto %d.\n", numVendedor, pedido.emisor, pedido.cantidad, pedido.tipoProducto);
	    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
	    
	    pedido_produccion_t pedidoProduccion = controlador.realizarPedido(pedido);
	    if(pedidoProduccion.producidoParaStockear + pedidoProduccion.producidoVendido > 0)
		controlador.enviarPedidoProduccionAAlmacenPiezas(pedidoProduccion);
	    
	    if(pedidoProduccion.ventaEnCurso)
		controlador.informarExitoEnPedido(pedido);
	    else
		controlador.informarErrorEnPedido(pedido);
	    
	} while(pedido.tipoProducto != FIN_ENVIO);
    }    
    
}

