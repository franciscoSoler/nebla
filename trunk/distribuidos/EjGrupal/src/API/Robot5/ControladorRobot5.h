/* 
 * File:   ControladorRobot5.h
 * Author: knoppix
 *
 * Created on October 22, 2013, 2:46 PM
 */

#ifndef CONTROLADORROBOT5_H
#define	CONTROLADORROBOT5_H

#include "../../IPCs/IPCAbstractos/MessageQueue/ComunicacionRobot5MessageQueue.h"

class ControladorRobot5 {
public:
    ControladorRobot5();
    virtual ~ControladorRobot5();
    
    void iniciarControlador();
    
    PedidoRobot5 obtenerPedido();
    
    void resolverPedido(Canasto canasto, int idDestino);
    void resolverPedido(ProductoEnProduccion prodEnProduccion, bool ultimo);
    
     Canasto obtenerCanasto(TipoPieza tipoPieza);
     Gabinete obtenerGabinete(TipoProducto tipoPorudcto);

private:
    IPC::ComunicacionRobot5MessageQueue comunicacionMsgQueue;
};

#endif	/* CONTROLADORROBOT5_H */

