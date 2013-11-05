#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../Common.h"
#include "ControladorCliente.h"

int main(int argc, char** argv)
{
    srand(time(NULL) + getpid());
    long numCliente = rand() % CANT_CLIENTES + 1;
    
    ControladorCliente controlador(numCliente);
    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "Se inicia el cliente #%ld.", numCliente);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    controlador.contactarVendedores();
    
    /* Envío los pedidos. */
    int cantPedidos = 1 + rand() % 3;
    sprintf(mensajePantalla,"Comienzo a enviar pedidos: %d",cantPedidos);
    Logger::getInstance().logMessage(Logger::TRACE,mensajePantalla);
    for(int i = 0; i < cantPedidos; i++)
    {
	int cantUnidades = 1 + rand() % CANT_MAX_PEDIDOS;
	int tipoProducto = rand() % CANT_PRODUCTOS;
	controlador.enviarPedido(cantUnidades, tipoProducto, i);
	if(controlador.recibirResultado() == false)
	{
	    exit(-1);
	}
    }
    
    controlador.finalizarEnvio(cantPedidos);
    bool pedidoEsValido = controlador.recibirResultado();
    if(!pedidoEsValido)
	exit(-1);
    
    // Cliente espera una orden de pedidos terminada
    TipoProducto tipo;
    int nroOrden;
    controlador.retirarEncargo(tipo, nroOrden);
    
    for (int i = 0; i < /*?? Acà va la cantidad pedida de tipo*/1 ;++i) {
        controlador.obtenerProducto(nroOrden);
    }
}
