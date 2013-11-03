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
    virtual void enviarPedidoProduccionARobot5(consulta_almacen_piezas_t consulta) = 0;
    virtual void obtenerEspecificacionesDelProducto(TipoProducto tipoPieza, EspecifProd piezasProductoActual) = 0;
    virtual void avisarAAGVQueAgregueCanasto(TipoProducto tipoPieza, EspecifProd piezasReservadasTemporalmente[2]) = 0;
    virtual void recibirConfirmacionProduccion() = 0;
    virtual ~IControladorAlmacenPiezas() {};
};

#endif	/* ICONTROLADORALMACENPIEZAS_H */

