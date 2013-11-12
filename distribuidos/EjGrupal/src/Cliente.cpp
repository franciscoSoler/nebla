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
    int llamando = rand() % MAX_DEMORA + 1;
    char mensajePantalla[256];
    ControladorCliente controlador(numCliente);
    int cantidadDeProductosPorPedido[CANTIDAD_PRODUCTOS];

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
        int cantUnidades = 1; //+ rand() % CANT_MAX_PEDIDOS;
        int tipoProducto = PRODUCTO_3;// rand() % CANT_PRODUCTOS + 1;
        int retardo = rand() % MAX_DEMORA + 1;

        cantidadDeProductosPorPedido[tipoProducto-1] = cantUnidades;

        usleep(retardo * 100 * 1000);
        controlador.enviarPedido(cantUnidades, tipoProducto);
        if(controlador.recibirResultado().recepcionOK == false) {
            Logger::logMessage(Logger::IMPORTANT, "APT lleno. No se pudo realizar el pedido");
            exit(0);
        }
    }
    
    controlador.finalizarEnvio(cantPedidos);
    respuesta_pedido_t respuesta = controlador.recibirResultado();

    if(! respuesta.recepcionOK) {
        Logger::logMessage(Logger::IMPORTANT, "APT lleno. No se pudo realizar el pedido");
        exit(0);
    }

    // Cliente espera a que lo llamen por las Productos que se van terminando.
    // Por cada uno de los Productos, recibe un mensaje, y lo va a buscar a la
    // fábrica. En la misma, recibe las Cajas que representan a los productos,
    // uno por uno
    TipoProducto tipo;
    int nroOrden;

    for (int i = 0; i < cantPedidos; ++i) {

        controlador.esperarPedido(tipo, nroOrden, numCliente);
        controlador.retirarEncargo(tipo, nroOrden);

        sprintf(mensajePantalla, "Cantidad de Productos N°%d a retirar:%d",
                tipo, cantidadDeProductosPorPedido[tipo-1]);
        Logger::logMessage(Logger::IMPORTANT, mensajePantalla);

        for (int j = 0; j < cantidadDeProductosPorPedido[tipo-1]; ++j) {
            controlador.obtenerProducto(numCliente);
        }
    }

    Logger::logMessage(Logger::IMPORTANT, "Abandona la planta luego de retirar su pedido");
}

