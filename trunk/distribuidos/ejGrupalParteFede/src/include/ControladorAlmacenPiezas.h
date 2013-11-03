/* 
 * File:   ControladorAlmacenPiezas.h
 * Author: fede
 *
 * Created on November 2, 2013, 1:47 PM
 */

#ifndef CONTROLADORALMACENPIEZAS_H
#define	CONTROLADORALMACENPIEZAS_H

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include "Cola.h"
#include "Parser.h"
#include "const.h"

class ControladorAlmacenPiezas
{
    public:
	ControladorAlmacenPiezas();
	virtual ~ControladorAlmacenPiezas();
	consulta_almacen_piezas_t recibirConsulta();
	int obtenerCantidadMinimaDeProduccion(int numProductoConsultado);
	//int obtenerComposicion(int numProductoConsultado);
	void responderConsulta(respuesta_almacen_piezas_t respuesta, int numEmisor);
	void enviarPedidoProduccionARobot5(consulta_almacen_piezas_t consulta);
    
    private:
	Cola<consulta_almacen_piezas_t> consultasAlmacen;
	Cola<respuesta_almacen_piezas_t> respuestasAlmacen;
	Cola<MensajePedidoProduccion> mensajesRobot5;
	
	void buscarUbiacionDeProductoEnArchivo(Parser parser, ifstream& stream, int numProducto);

};

#endif	/* CONTROLADORALMACENPIEZAS_H */

