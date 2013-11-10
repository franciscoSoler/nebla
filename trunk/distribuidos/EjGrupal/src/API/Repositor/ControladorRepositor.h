/* 
 * File:   ControladorRepositor.h
 * Author: fede
 *
 * Created on November 9, 2013, 5:30 PM
 */

#ifndef CONTROLADORREPOSITOR_H
#define	CONTROLADORREPOSITOR_H

#include "../../Common.h"
#include "../../IPCs/IPCAbstractos/SharedMemory/EspacioAlmacenPiezasSharedMemory.h"
#include "../../IPCs/Semaphore/Semaphore.h"

class ControladorRepositor
{
    public:
	ControladorRepositor();
	virtual ~ControladorRepositor();
	
	void reponerCanastos(int numeroPieza);
	void reponerGabinetes(int numeroGabinete);
	
	void iniciarControlador();
	
    private:
	IPC::EspacioAlmacenPiezasSharedMemory almacenSharedMemory;
	IPC::Semaphore bloqueoControlador;
	IPC::Semaphore mutex;
};

#endif	/* CONTROLADORREPOSITOR_H */

