/* 
 * File:   ControladorRobot5Cinta.h
 * Author: knoppix
 *
 * Created on November 9, 2013, 1:17 PM
 */

#ifndef CONTROLADORROBOT5CINTA_H
#define	CONTROLADORROBOT5CINTA_H

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

#include "../CintaTransportadora/CintaTransportadora6.h"

#include "../../IPCs/IPCAbstractos/MessageQueue/PedidosProduccionMessageQueue.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/EstadoRobot5SharedMemory.h"
#include <Comunicaciones/Objects/CommSemaphoreMutex.h>

#include "../AlmacenPiezas/EspacioAlmacenPiezas/EspacioAlmacenPiezas.h"

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

    void leerEstadoCintas(EstadoCinta &estadoCinta0,EstadoCinta &estadoCinta1);

    CintaTransportadora6 cintaTransportadora[2];
    
    IPC::PedidosProduccionMessageQueue colaPedidosProduccion;
    IPC::EstadoRobot5SharedMemory estadoRobot5;
    COMM::CommSemaphoreMutex<EstadoRobot5> semaforoAccesoEstadoRobot5;
    COMM::CommSemaphore semaforoBloqueoRobot5;

    COMM::CommSemaphore semaforoBloqueoRobot11;
    
    IPC::Semaphore semaforoApiRobot5;
    
    EspacioAlmacenPiezas almacen;
};

#endif	/* CONTROLADORROBOT5CINTA_H */

