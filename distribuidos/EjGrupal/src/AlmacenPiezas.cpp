/* 
 * File:   AlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 1:46 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "../../Common.h"
#include "ControladorAlmacenPiezas.h"

int main(int argc, char** argv)
{
    ControladorAlmacenPiezas controlador;
    
    while(true)
    {
	consulta_almacen_piezas_t consulta = controlador.recibirConsulta();
	
	if(consulta.tipoConsulta == CANT_MINIMA_FABRICACION)
	{
	    respuesta_almacen_piezas_t respuesta;
	    respuesta.cantidad = controlador.obtenerCantidadMinimaDeProduccion(consulta.tipoProducto);
	    controlador.responderConsulta(respuesta, consulta.emisor);	
	}
	
	else if(consulta.tipoConsulta == PRODUCTOS_NECESARIOS)
	{
	    // doStuff();
	}
	
	else if(consulta.tipoConsulta == ORDEN_PRODUCCION)
	    controlador.enviarPedidoProduccionARobot5(consulta);
	
    }   
}