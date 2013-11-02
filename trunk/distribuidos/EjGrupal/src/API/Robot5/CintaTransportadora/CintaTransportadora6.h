/* 
 * File:   CintaTransportadora6.h
 * Author: knoppix
 *
 * Created on October 31, 2013, 9:48 PM
 */

#ifndef CINTATRANSPORTADORA6_H
#define	CINTATRANSPORTADORA6_H

#include "../../../IPCs/Duran/SharedMemory/CintaTransportadoraSharedMemory.h"
#include "../../../IPCs/Semaphore/Semaphore.h"

typedef struct {
    bool ocupado;
    int cantOcupados;
} EstadoCinta;

class CintaTransportadora6 {
public:
    
    CintaTransportadora6();
    CintaTransportadora6(int idCinta, int idSemCinta);
    virtual ~CintaTransportadora6();
    
    void iniciarCinta(int idClave);
    void depositarProductoEnProduccion(ProductoEnProduccion producto);
    bool obtenerEstadoRobot11();
    EstadoCinta obtenerEstadoCinta();
        
private:
    CintaTransportadora6(const CintaTransportadora6& orig);
    
    int idCinta;  
    IPC::Semaphore semaforoAcceso;
    IPC::CintaTransportadoraSharedMemory cinta;
    
    void mostrarEstadoCinta(CintaTransportadora_6 cintaTransportadora);
       
};

#endif	/* CINTATRANSPORTADORAINICIAL_H */