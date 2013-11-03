/* 
 * File:   ControladorAlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 1:47 PM
 */

#include "ControladorAlmacenPiezas.h"

ControladorAlmacenPiezas::ControladorAlmacenPiezas()
{ 
    this->consultasAlmacen = Cola<consulta_almacen_piezas_t>(NOMBRE_ARCHIVO, LETRA_COLA_CONSULTAS_ALMACEN_PIEZAS);
    consultasAlmacen.obtener();
    this->respuestasAlmacen = Cola<respuesta_almacen_piezas_t>(NOMBRE_ARCHIVO, LETRA_COLA_RESPUESTAS_ALMACEN_PIEZAS);
    respuestasAlmacen.obtener();
    this->mensajesRobot5 = Cola<MensajePedidoProduccion>(NOMBRE_ARCHIVO_ROBOT5, LETRA_ROBOT5);
    mensajesRobot5.obtener();
}

ControladorAlmacenPiezas::~ControladorAlmacenPiezas() { }

consulta_almacen_piezas_t ControladorAlmacenPiezas::recibirConsulta()
{
    char mensajePantalla[256];
    consulta_almacen_piezas_t buffer;
    consultasAlmacen.recibir(0, &buffer);
    long numVendedor = buffer.emisor;
    int numProductoConsultado = buffer.tipoProducto;
    sprintf(mensajePantalla, "Almacén recibe consulta de producto %d por vendedor #%ld.\n", numProductoConsultado, numVendedor);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    return buffer;
}

int ControladorAlmacenPiezas::obtenerCantidadMinimaDeProduccion(int numeroProducto)
{
    ifstream stream;
    stream.open(NOMBRE_ARCHIVO_PRODUCTOS);
    Parser parser;
    this->buscarUbiacionDeProductoEnArchivo(parser, stream, numeroProducto);
    string cantMinimaProduccionString = parser.obtenerProximoValor();
    int cantMinimaProduccion = atoi(cantMinimaProduccionString.c_str());
    
    char mensajePantalla[256];
    sprintf(mensajePantalla, "Almacén dice que la cantidad mínima de producción de producto %d es %d.\n", numeroProducto, cantMinimaProduccion);
    write(fileno(stdout), mensajePantalla, strlen(mensajePantalla));
    
    return cantMinimaProduccion;
}

/*
int ControladorAlmacenPiezas::obtenerComposicion(int numeroProducto)
{
    ifstream stream;
    stream.open(NOMBRE_ARCHIVO_PRODUCTOS);
    Parser parser;
    this->buscarUbiacionDeProductoEnArchivo(parser, stream, numeroProducto);
    parser.obtenerProximoValor(); / cantidad mínima de producción. *
    string numeroComponentesString = parser.obtenerProximoValor();
    int numeroComponentes = atoi(numeroComponentesString.c_str());
    
    int* componentes = (int*) malloc(sizeof(int) * (CANT_MAX_COMPONENTES_PRODUCTO*2 + 1));
    componentes[0] = numeroComponentes;
	    
    for(int i = 1; i < numeroComponentes*2 + 2; i++)
    {
	string valorString = parser.obtenerProximoValor();
	int valor = atoi(valorString.c_str());
	componentes[i] = valor;
    }
    
    return componentes;    
}*/

void ControladorAlmacenPiezas::buscarUbiacionDeProductoEnArchivo(Parser parser, ifstream& stream, int numeroProducto)
{
    bool continuaArchivo = true;
    int ultimoNumeroProductoLeido = 0;
    do
    {
	continuaArchivo = parser.obtenerLineaSiguiente(stream);
	if(!continuaArchivo)
	    continue;
	string ultimoNumeroProductoLeidoString = parser.obtenerProximoValor();
	ultimoNumeroProductoLeido = atoi(ultimoNumeroProductoLeidoString.c_str());
    } while(continuaArchivo && ultimoNumeroProductoLeido != numeroProducto);
}

void ControladorAlmacenPiezas::enviarPedidoProduccionARobot5(consulta_almacen_piezas_t consulta)
{
    PedidoProduccion pedidoProduccion;
    pedidoProduccion.cantidad = consulta.cantidad;
    pedidoProduccion.diferenciaMinima = consulta.diferencia;
    pedidoProduccion.nroOrdenCompra = consulta.numOrdenCompra;
    switch(consulta.tipoProducto)
    {
	case PRODUCTO_1:
	    pedidoProduccion.tipo = _PRODUCTO_1;
	    break;
	case PRODUCTO_2:
	    pedidoProduccion.tipo = _PRODUCTO_2;
	    break;
	case PRODUCTO_3:
	    pedidoProduccion.tipo = _PRODUCTO_3;
	    break;    
    }
    
    MensajePedidoProduccion mensajePedidoProduccion;
    mensajePedidoProduccion.pedidoProduccion = pedidoProduccion;
    mensajePedidoProduccion.mtype = TIPO_ROBOT_5;
    
    mensajesRobot5.enviar(mensajePedidoProduccion);    
}

void ControladorAlmacenPiezas::responderConsulta(respuesta_almacen_piezas_t respuesta, int numEmisor)
{
    respuesta.emisor = 0;
    respuesta.mtype = numEmisor;
    respuestasAlmacen.enviar(respuesta);
}

