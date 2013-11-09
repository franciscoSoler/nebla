/* 
 * File:   ControladorRobot5Cinta.h
 * Author: knoppix
 *
 * Created on November 9, 2013, 1:17 PM
 */

#ifndef CONTROLADORROBOT5CINTA_H
#define	CONTROLADORROBOT5CINTA_H

#include "../CintaTransportadora/CintaTransportadora6.h"

#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"
#include "../../IPCs/Semaphore/Semaphore.h"

class ControladorRobot5Cinta {

public:
    ControladorRobot5Cinta();
    virtual ~ControladorRobot5Cinta();

    void iniciarControlador();

    PedidoProduccion obtenerPedidoProduccion();
    void obtenerPedidoGabinete();
    
    void avisarProximoPedido();

    void resolverPedidoGabinete(ProductoEnProduccion prodEnProduccion, int cintaAUtilizar);

    Gabinete obtenerGabinete(TipoProducto tipoPorudcto);
    TipoGabinete obtenerTipoGabinete(TipoProducto tipoProducto);
    
    EstadoCinta obtenerEstadoCinta(int numeroCinta);
    
private:
    ControladorRobot5Cinta(const ControladorRobot5Cinta& orig);
    ControladorRobot5Cinta& operator= (const ControladorRobot5Cinta &p);

    CintaTransportadora6 cintaTransportadora[2];
    
    IPC::PedidosProduccionMessageQueue colaPedidosProduccion;
    IPC::EstadoRobot5SharedMemory estadoRobot5;
    IPC::Semaphore semaforoAccesoEstadoRobot5;
    IPC::Semaphore semaforoBloqueoRobot5;

    IPC::Semaphore semaforoBloqueoRobot11;
};

#endif	/* CONTROLADORROBOT5CINTA_H */

