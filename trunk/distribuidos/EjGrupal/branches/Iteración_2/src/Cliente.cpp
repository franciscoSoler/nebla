#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sstream>

#include "../../Common.h"
#include "ControladorCliente.h"
#include <API/Objects/Util.h>

#define MAX_DEMORA 5

int generarNumeroDeTipoDeProducto() {
    static int pedidosRealizados[CANTIDAD_PRODUCTOS];
    static int cantidadPedidosRealizados = 0;

    bool existeNumeroGenerado = true;
    int numeroAleatorio = Util::getInstance().generateRandomNumber(0, CANTIDAD_PRODUCTOS-1);

    while( existeNumeroGenerado ) {
        existeNumeroGenerado = false;
        numeroAleatorio = Util::getInstance().generateRandomNumber(0, CANTIDAD_PRODUCTOS-1);

        for (int i = 0; i < cantidadPedidosRealizados; ++i) {
            if (numeroAleatorio == pedidosRealizados[i]) {
                existeNumeroGenerado = true;
                break;
            }
        }
        // Espera para darle tiempo al sleep que calcule otro número
        sleep(1);
    }

    pedidosRealizados[cantidadPedidosRealizados] = numeroAleatorio;
    cantidadPedidosRealizados++;
    return numeroAleatorio+1;
}

int main(int argc, char** argv)
{
    long numCliente = 0;
    char mensajePantalla[TAM_BUFFER];

    std::stringstream ss;
    ss << argv[1];
    ss >> numCliente;

    ControladorCliente controlador(numCliente);
    //numCliente = controlador.obtenerNumeroCliente();

    if (numCliente == 0) {
        sprintf(mensajePantalla, "No se puo obtener un numero de cliente (vendedores ocupados).");
        Logger::logMessage(Logger::DEBUG, mensajePantalla);
        return 0;
    }            
    
    int llamando = Util::getInstance().generateRandomNumber(1, MAX_DEMORA);
    
    int cantidadDeProductosPorPedido[CANTIDAD_PRODUCTOS];

    sprintf(mensajePantalla, "Se inicia el cliente #%ld.", numCliente);
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    usleep(llamando * 1000 * 1000);
    
    //controlador.contactarVendedores();
    
    /* Envio los pedidos. */
    int cantPedidos = Util::getInstance().generateRandomNumber(1, 3);
    sprintf(mensajePantalla,"Comienzo a enviar sus %d pedidos.", cantPedidos);
    Logger::getInstance().logMessage(Logger::TRACE, mensajePantalla);

    for(int i = 0; i < cantPedidos; i++)
    {
        int cantUnidades = Util::getInstance().generateRandomNumber(1, 5);
        int tipoProducto = generarNumeroDeTipoDeProducto();
        int retardo = rand() % MAX_DEMORA + 1;
        cantidadDeProductosPorPedido[tipoProducto-1] = cantUnidades;

        sprintf(mensajePantalla, "Cliente realiza pedido - TipoProducto: %d - Cantidad: %d",
                tipoProducto, cantUnidades);
        Logger::logMessage(Logger::IMPORTANT, mensajePantalla);

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
        Logger::logMessage(Logger::IMPORTANT, "Procede a retirar un pedido");

        controlador.esperarPedido(tipo, nroOrden, numCliente);
        bool ultimoPedido = (i+1) == cantPedidos;
        controlador.retirarEncargo(tipo, nroOrden, ultimoPedido);

        sprintf(mensajePantalla, "Cantidad de Productos N°%d a retirar:%d",
                tipo, cantidadDeProductosPorPedido[tipo-1]);
        Logger::logMessage(Logger::IMPORTANT, mensajePantalla);

        for (int j = 0; j < cantidadDeProductosPorPedido[tipo-1]; ++j) {
            controlador.obtenerProducto(numCliente);
        }
    }

    Logger::logMessage(Logger::IMPORTANT, "Abandona la planta luego de retirar su pedido");
}

