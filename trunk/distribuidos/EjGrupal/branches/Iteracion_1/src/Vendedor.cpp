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
   
    
    sprintf(mensajePantalla, "Se inicia el vendedor.");
    Logger::logMessage(Logger::TRACE, mensajePantalla);
    
    while(true)
    {
        sprintf(mensajePantalla, "Espera clientes.");
        Logger::logMessage(Logger::TRACE, mensajePantalla);

        controlador.recibirLlamadoTelefonico();

        OrdenDeCompra ordenDeCompra = obtenerNuevaOrdenDeCompra(controlador.obtenerNumeroDeOrdenDeCompra(), numVendedor);

        pedido_t pedido;
        pedido_t pedidos[CANT_MAX_PEDIDOS];
        long numCliente;
        int cantPedidos = 0;

        do
        {
            pedido = controlador.recibirPedido();
            numCliente = pedido.emisor;

            ordenDeCompra.idCliente_ = numCliente;

            pedidos[cantPedidos] = pedido;
            respuesta_pedido_t respuesta;
            respuesta.recepcionOK = true;
            controlador.enviarConfirmacionDeRecepcionDePedido(numCliente, respuesta);
            if(pedido.fin == true)
            continue;
            cantPedidos++;
        } while(pedido.fin == false);

        Logger::getInstance().logMessage(Logger::TRACE, "Fin de recepcion de pedidos del cliente.");

        bool pedidoEsValido = controlador.realizarOrdenDeCompra(pedidos, &ordenDeCompra, cantPedidos);
        if(pedidoEsValido)
        {   
            Logger::getInstance().logMessage(Logger::TRACE, "Pedido Valido");
            respuesta_pedido_t respuesta;
            respuesta.recepcionOK = true;

            for (int i = 0; i < CANTIDAD_PRODUCTOS; ++i) {
                respuesta.cantidadDeProductos[i] = ordenDeCompra.cantidadPorProducto_[i];
            }

            controlador.enviarConfirmacionDeRecepcionDePedido(numCliente, respuesta);
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

