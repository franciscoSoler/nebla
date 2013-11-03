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
#include "../../IPCs/Barrios/Cola.h"
#include "../../Parser/Parser.h"
#include "../../Common.h"
#include "IControladorAlmacenPiezas.h"
#include "../../IPCs/Semaphore/Semaphore.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosAgvMessageQueue.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/BufferCanastosSharedMemory.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosCanastosMessageQueue.h"
#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"

class ControladorAlmacenPiezas : public IControladorAlmacenPiezas
{
    public:
	ControladorAlmacenPiezas();
	virtual ~ControladorAlmacenPiezas();
	consulta_almacen_piezas_t recibirConsulta();
	int obtenerCantidadMinimaDeProduccion(int numProductoConsultado);
	//int obtenerComposicion(int numProductoConsultado);
	void responderConsulta(respuesta_almacen_piezas_t respuesta, int numEmisor);
	void enviarPedidoProduccionARobot5(consulta_almacen_piezas_t consulta);
        
        void obtenerEspecificacionesDelProducto(TipoProducto tipoProducto, EspecifProd piezasProductoActual);
        void avisarAAGVQueAgregueCanasto(TipoPieza tipoPieza, EspecifProd piezasReservadasTemporalmente[2]);
        void recibirConfirmacionProduccion();
    
    private:
        char buffer[TAM_BUFFER];
	Cola<consulta_almacen_piezas_t> consultasAlmacen;
	Cola<respuesta_almacen_piezas_t> respuestasAlmacen;
        
        IPC::PedidosProduccionMessageQueue mensajesRobot5;
	//Cola<MensajePedidoProduccion> mensajesRobot5;
        IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
        IPC::BufferCanastosSharedMemory shMemBufferCanastos[CANTIDAD_AGVS];
        IPC::Semaphore semMemCanastos;
	
	void buscarUbiacionDeProductoEnArchivo(Parser parser, ifstream& stream, int numProducto);

};

#endif	/* CONTROLADORALMACENPIEZAS_H */

