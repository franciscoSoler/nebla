/* 
 * File:   CintaTransportadora6.h
 * Author: knoppix
 *
 * Created on October 31, 2013, 9:48 PM
 */

#ifndef CINTATRANSPORTADORA6_H
#define	CINTATRANSPORTADORA6_H

#include "../../../IPCs/IPCAbstractos/SharedMemory/Cinta6SharedMemory.h"
#include "Comunicaciones/Objects/CommSemaphoreMutex.h"

typedef struct {
    bool ocupado;
    int cantOcupados;
} EstadoCinta;

class CintaTransportadora6 {
public:
    
    CintaTransportadora6();
    CintaTransportadora6(int idCinta);
    virtual ~CintaTransportadora6();
    
    void iniciarCinta(int idClave, int idClaveSem, int idCinta);
    void depositarProductoEnProduccion(ProductoEnProduccion producto);
    EstadoCinta obtenerEstadoCinta();
    
    bool robot11Bloqueado();
    void marcarRobot11Liberado();
    std::string obtenerMensajeEstado();
    
        
private:
    CintaTransportadora6(const CintaTransportadora6& orig);
    
    int idCinta;  
    COMM::CommSemaphoreMutex<CintaTransportadora_6> semaforoAcceso;
    IPC::Cinta6SharedMemory cinta;
    
    void mostrarEstadoCinta(CintaTransportadora_6 cintaTransportadora);
       
};

#endif	/* CINTATRANSPORTADORAINICIAL_H */