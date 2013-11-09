/* 
 * File:   ShMemAlmacenDePiezas.h
 * Author: fede
 *
 * Created on November 9, 2013, 12:37 PM
 */

#ifndef SHMEMALMACENDEPIEZAS_H
#define	SHMEMALMACENDEPIEZAS_H

#include "../AbstractSharedMemory/AbstractSharedMemory.h"
#include "../../Common.h"
#include "../Semaphore/Semaphore.h"

namespace IPC {

    class ShMemAlmacenDePiezas : public AbstractSharedMemory
    {
	public:
	    ShMemAlmacenDePiezas(std::string IPCName = "");
	    virtual ~ShMemAlmacenDePiezas();
	    Canasto obtenerCanastoConPiezas(int numeroPieza);

	protected:
	    size_t getMemorySize();
	    
	private:
	    IPC::Semaphore mutex;
	    void reponerPiezas(int numeroPieza);
	    int* almacen;
	    char buffer[1024];
    };

}
#endif	/* SHMEMALMACENDEPIEZAS_H */

