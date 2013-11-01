/* 
 * File:   CintaTransportadoraInicial.h
 * Author: knoppix
 *
 * Created on October 31, 2013, 9:48 PM
 */

#ifndef CINTATRANSPORTADORAINICIAL_H
#define	CINTATRANSPORTADORAINICIAL_H

#include "IPC/SharedMemory/CintaTransportadoraSharedMemory.h"
#include "IPC/Semaphore/Semaphore.h"


typedef struct {
    bool ocupado;
    int cantOcupados;
} EstadoCinta;

class CintaTransportadoraInicial {
public:
    
    CintaTransportadoraInicial();
    CintaTransportadoraInicial(int idCinta);
    virtual ~CintaTransportadoraInicial();
    
    void iniciarCinta(int idClaveMemoria, int idClaveSemaforo);
    void depositarProductoEnProduccion(ProductoEnProduccion producto);
    EstadoCinta obtenerEstadoCinta();
        
private:
    CintaTransportadoraInicial(const CintaTransportadoraInicial& orig);
    
    int idCinta;  
    Semaphore semaforoAcceso;
    CintaTransportadoraSharedMemory cinta;
    
    void mostrarEstadoCinta(CintaTransportadora_6 cintaTransportadora);
       
};

#endif	/* CINTATRANSPORTADORAINICIAL_H */

