/* 
 * File:   IControladorAlmacenPiezas.h
 * Author: francisco
 *
 * Created on 3 de noviembre de 2013, 10:26
 */

#ifndef ICONTROLADORALMACENPIEZAS_H
#define	ICONTROLADORALMACENPIEZAS_H

#include "../Common.h"

class IControladorAlmacenPiezas
{
public:
    virtual void obtenerEspecificacionesDelProducto(TipoProducto tipoPieza, EspecifProd &piezasProductoActual) = 0;
    virtual void avisarAAGVQueAgregueCanasto(TipoPieza tipoPieza, EspecifProd piezasReservadasTemporalmente[2]) = 0;
    virtual void recibirConfirmacionProduccion() = 0;
    virtual pedido_fabricacion_t recibirPedidoDeFabricacion() = 0;
    virtual void enviarPedidoProduccionARobot5(pedido_fabricacion_t pedidoFabricacion) = 0;
    virtual ~IControladorAlmacenPiezas() {};
};

#endif	/* ICONTROLADORALMACENPIEZAS_H */

