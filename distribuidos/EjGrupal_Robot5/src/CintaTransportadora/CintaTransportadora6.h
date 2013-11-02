/* 
 * File:   CintaTransportadora6.h
 * Author: knoppix
 *
 * Created on October 31, 2013, 9:48 PM
 */

#ifndef CINTATRANSPORTADORA6_H
#define	CINTATRANSPORTADORA6_H

#include "IPC/SharedMemory/CintaTransportadoraSharedMemory.h"
#include "IPC/Semaphore/Semaphore.h"


typedef struct {
    bool ocupado;
    int cantOcupados;
} EstadoCinta;

class CintaTransportadora6 {
public:
    
    CintaTransportadora6();
    CintaTransportadora6(int idCinta);
    virtual ~CintaTransportadora6();
    
    void iniciarCinta(int idClave);
    void depositarProductoEnProduccion(ProductoEnProduccion producto);
    EstadoCinta obtenerEstadoCinta();
        
private:
    CintaTransportadora6(const CintaTransportadora6& orig);
    
    int idCinta;  
    Semaphore semaforoAcceso;
    CintaTransportadoraSharedMemory cinta;
    
    void mostrarEstadoCinta(CintaTransportadora_6 cintaTransportadora);
       
};

#endif	/* CINTATRANSPORTADORAINICIAL_H */

