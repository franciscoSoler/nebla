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
#include "LockFile.h"

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

    sprintf(mensajePantalla,"Vendedor %ld",numVendedor);
    Logger::getInstance().setProcessInformation(mensajePantalla);
   
    
    sprintf(mensajePantalla, "Se inicia el vendedor.\n");
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    while(true)
    {
	sprintf(mensajePantalla, "Espera clientes.\n");
        Logger::logMessage(Logger::TRACE, mensajePantalla);
	
	long numCliente = controlador.recibirLlamadoTelefonico();
	
	sprintf(mensajePantalla, "Recibe mensaje del cliente %ld y establece una comunicación.\n", numCliente);
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
	
        Logger::getInstance().logMessage(Logger::DEBUG, "3");
        
	bool pedidoEsValido = controlador.realizarOrdenDeCompra(pedidos, &ordenDeCompra, cantPedidos);
	if(pedidoEsValido)
	{
            Logger::getInstance().logMessage(Logger::TRACE, "Pedido Valido");
	    respuesta_pedido_t respuesta;
	    respuesta.recepcionOK = true;
	    controlador.confirmarOrdenDeCompraACliente(numCliente, respuesta);
	}
	else
	{
            Logger::getInstance().logMessage(Logger::TRACE, "Pedido Invalido");
	    respuesta_pedido_t respuesta;
	    respuesta.recepcionOK = false;
	    controlador.cancelarOrdenDeCompraACliente(numCliente, respuesta);
	}

    }  
    
}

