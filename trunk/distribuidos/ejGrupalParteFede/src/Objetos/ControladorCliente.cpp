/* 
 * File:   ControladorCliente.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 12:58 AM
 */

#include "ControladorCliente.h"

ControladorCliente::ControladorCliente() { }

ControladorCliente::ControladorCliente(long numCliente)
{ 
    this->vendedores = Cola<mensaje_inicial_t>(NOMBRE_ARCHIVO, LETRA_COLA_VENDEDORES);
    this->vendedores.obtener();
    this->clientes = Cola<mensaje_inicial_t>(NOMBRE_ARCHIVO, LETRA_COLA_CLIENTES);
    this->clientes.obtener();
    
    this->numCliente = numCliente;
}

void ControladorCliente::contactarVendedores()
{
    char mensajePantalla[256];
    /* Se establece la comunicación "en dos pasos". */
    mensaje_inicial_t mensaje;
    mensaje.emisor = numCliente;
    mensaje.mtype = CANT_VENDEDORES;
    sprintf(mensajePantalla, "Cliente #%ld llama a algún vendedor.\n", numCliente);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    vendedores.enviar(mensaje);    
    
    mensaje_inicial_t respuesta;
    clientes.recibir(numCliente, &respuesta);
    long numVendedor = respuesta.emisor;
    sprintf(mensajePantalla, "Cliente #%ld recibe la respuesta del vendedor.\n", numCliente);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    
    pedidos = Cola<pedido_t>(NOMBRE_ARCHIVO, LETRA_COLA_CLIENTES + (char) 1 + (char) numVendedor);
    pedidos.obtener();
    
    this->numVendedorAsociado = numVendedor;
}

void ControladorCliente::enviarPedido(int cantidadUnidades, int tipo, int numMensaje)
{
    char mensajePantalla[256];
    pedido_t pedido;
    pedido.emisor = numCliente;
    pedido.cantidad = cantidadUnidades;
    pedido.mtype = numVendedorAsociado;
    pedido.tipo = tipo;
    pedido.numeroMensaje = numMensaje + 1;
    sprintf(mensajePantalla, "Cliente #%ld envía al vendedor %ld un pedido por %d productos de tipo %d.\n", numCliente, numVendedorAsociado, pedido.cantidad, pedido.tipo);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    pedidos.enviar(pedido);	    
}

void ControladorCliente::finalizarEnvio(int cantPedidos)
{
    char mensajePantalla[256];
    
    /* Envío el mensaje final. */
    pedido_t pedidoFinal;
    pedidoFinal.emisor = numCliente;
    pedidoFinal.cantidad = 0;
    pedidoFinal.mtype = numVendedorAsociado;
    pedidoFinal.tipo = FIN_ENVIO;
    pedidoFinal.numeroMensaje = cantPedidos + 1;
    sprintf(mensajePantalla, "Cliente #%ld envía al vendedor %ld el mensaje de fin de pedido.\n", numCliente, numVendedorAsociado);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    pedidos.enviar(pedidoFinal);
}

ControladorCliente::~ControladorCliente() { }

