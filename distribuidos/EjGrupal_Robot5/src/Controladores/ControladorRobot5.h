/* 
 * File:   ControladorRobot5.h
 * Author: knoppix
 *
 * Created on October 22, 2013, 2:46 PM
 */

#ifndef CONTROLADORROBOT5_H
#define	CONTROLADORROBOT5_H

#include "../IPC/MessageQueue/ComunicacionRobot5MessageQueue.h"

class ControladorRobot5 {
public:
    ControladorRobot5();
    virtual ~ControladorRobot5();
    
    void iniciarControlador();
    
    PedidoRobot5 obtenerPedido();
    
    void resolverPedido(Canasto canasto, int idDestino);
    void resolverPedido(Gabinete gabinete, bool ultimo, int ordenDeCompra);
    
     Canasto obtenerCanasto(TipoPieza tipoPieza);
     Gabinete obtenerGabinete(TipoProducto tipoPorudcto);

private:
    ComunicacionRobot5MessageQueue comunicacionMsgQueue;
};

#endif	/* CONTROLADORROBOT5_H */

