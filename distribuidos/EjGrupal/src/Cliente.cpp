#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../Common.h"
#include "ControladorCliente.h"

#define MAX_DEMORA 5

int main(int argc, char** argv)
{
    srand(time(NULL) + getpid());
    long numCliente = rand() % CANT_CLIENTES + 1;
    
    ControladorCliente controlador(numCliente);
    int llamando = rand() % MAX_DEMORA + 1;
    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "Se inicia el cliente #%ld.", numCliente);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    usleep(llamando * 1000 * 1000);
    
    controlador.contactarVendedores();
    
    /* Envio los pedidos. */
    int cantPedidos = 1;// + rand() % 3;
    sprintf(mensajePantalla,"Comienzo a enviar sus %d pedidos.", cantPedidos);
    Logger::getInstance().logMessage(Logger::TRACE, mensajePantalla);
    for(int i = 0; i < cantPedidos; i++)
    {
	int cantUnidades = 1 + rand() % CANT_MAX_PEDIDOS;
	int tipoProducto = PRODUCTO_3;// rand() % CANT_PRODUCTOS + 1;
	int retardo = rand() % MAX_DEMORA + 1;
	usleep(retardo * 100 * 1000);
	controlador.enviarPedido(cantUnidades, tipoProducto);
	if(controlador.recibirResultado() == false)
	    exit(-1);
    }
    
    controlador.finalizarEnvio(cantPedidos);
    bool pedidoEsValido = controlador.recibirResultado();
    if(!pedidoEsValido)
	exit(-1);
    
    // Cliente espera una orden de pedidos terminada
    TipoProducto tipo;
    int nroOrden;
    controlador.retirarEncargo(tipo, nroOrden);
    
    for (int i = 0; i < cantPedidos; ++i) {
        controlador.obtenerProducto(nroOrden);
    }
}

