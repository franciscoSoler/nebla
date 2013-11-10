/* 
 * File:   EspacioAlmacenPiezas.h
 * Author: fede
 *
 * Created on November 9, 2013, 10:40 PM
 */

#ifndef ESPACIOALMACENPIEZAS_H
#define	ESPACIOALMACENPIEZAS_H

#include "../../../IPCs/Semaphore/Semaphore.h"
#include "../../../IPCs/IPCAbstractos/SharedMemory/EspacioAlmacenPiezasSharedMemory.h"

class EspacioAlmacenPiezas
{
public:
    EspacioAlmacenPiezas();
    
    void iniciarEspacioAlmacen();
    
    Canasto obtenerCanastoConPiezas(TipoPieza numeroPieza);
    Gabinete obtenerGabinete(TipoGabinete numeroGabinete);
    
    virtual ~EspacioAlmacenPiezas();
private:
    EspacioAlmacenPiezas(const EspacioAlmacenPiezas& orig);

    IPC::EspacioAlmacenPiezasSharedMemory espacioSharedMemory;
    IPC::Semaphore mutex;  
    IPC::Semaphore bloqueoControladorAlmacenPiezas;
    
};

#endif	/* ESPACIOALMACENPIEZAS_H */

