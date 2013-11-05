/* 
 * File:   AlmacenPiezas.cpp
 * Author: fede
 * 
 * Created on November 2, 2013, 1:46 PM
 */

#include <sstream>
#include <cstdlib>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "Common.h"
#include "API/AlmacenPiezas/ControladorAlmacenPiezas.h"
#include "API/AlmacenPiezas/IControladorAlmacenPiezas.h"
#include "API/AGV/ControladorAGV.h" 

int main(int argc, char** argv)
{
    std::auto_ptr<IControladorAlmacenPiezas> controladorAlmacenPiezas = std::auto_ptr<IControladorAlmacenPiezas>(new ControladorAlmacenPiezas());

    pedido_fabricacion_t pedidoFabricacion;
    EspecifProd piezasReservadasTemporalmente[2];
    EspecifProd piezasProductoActual;
    piezasReservadasTemporalmente[0].idProducto = NULL_PRODUCT;
    piezasReservadasTemporalmente[1].idProducto = NULL_PRODUCT;
    
    while (true) {
        pedidoFabricacion = controladorAlmacenPiezas->recibirPedidoDeFabricacion();

        controladorAlmacenPiezas->enviarPedidoProduccionARobot5(pedidoFabricacion);
        
        controladorAlmacenPiezas->obtenerEspecificacionesDelProducto(pedidoFabricacion.tipoProducto, piezasProductoActual);
        for (int j = 0; j < piezasProductoActual.cantPiezas; j++) 
            controladorAlmacenPiezas->avisarAAGVQueAgregueCanasto(piezasProductoActual.pieza[j].tipoPieza, piezasReservadasTemporalmente);
        controladorAlmacenPiezas->avisarAAGVQueAgregueCanasto(piezasProductoActual.tipoPantalla.tipoPieza, piezasReservadasTemporalmente);
        memcpy(piezasReservadasTemporalmente, piezasReservadasTemporalmente + 1, sizeof(EspecifProd));
        memcpy(piezasReservadasTemporalmente + 1, &piezasProductoActual, sizeof(EspecifProd));
        controladorAlmacenPiezas->recibirConfirmacionProduccion();
    } 
}

