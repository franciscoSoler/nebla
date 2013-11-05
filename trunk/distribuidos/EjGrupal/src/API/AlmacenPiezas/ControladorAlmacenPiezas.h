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
#include <fstream>


#include "../../Parser/Parser.h"
#include "../../Common.h"
#include "IControladorAlmacenPiezas.h"

#include "../../IPCs/Barrios/Cola.h"
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
	pedido_fabricacion_t recibirPedidoDeFabricacion();
	int obtenerCantidadMinimaDeProduccion(int numProductoConsultado);
	void responderConsulta(respuesta_almacen_piezas_t respuesta, int numEmisor);
        void obtenerEspecificacionesDelProducto(TipoProducto tipoProducto, EspecifProd &piezasProductoActual);
        void avisarAAGVQueAgregueCanasto(TipoPieza tipoPieza, EspecifProd piezasReservadasTemporalmente[2]);
        void recibirConfirmacionProduccion();
        void enviarPedidoProduccionARobot5(pedido_fabricacion_t pedidoFabricacion);

    
    private:
        char buffer[TAM_BUFFER];
        Cola<mensaje_pedido_fabricacion_t> colaReciboOrdenProduccion;
        
        IPC::PedidosProduccionMessageQueue colaEnvioMensajePedidoProduccion;
        IPC::PedidosCanastosMessageQueue colaPedidosCanastos;
        IPC::BufferCanastosSharedMemory shMemBufferCanastos[CANTIDAD_AGVS];
        IPC::Semaphore semMemCanastos;
	
	void buscarUbiacionDeProductoEnArchivo(Parser parser, ifstream& stream, int numProducto);

};

#endif	/* CONTROLADORALMACENPIEZAS_H */

