/* 
 * File:   Oxigeno.h
 * Author: distribuidos
 *
 * Created on April 12, 2013, 9:45 PM
 */

#ifndef OXIGENO_H
#define	OXIGENO_H
#include "IPC/Semaphore.h"
#include "IPC/SharedMemoryCantidadHidrogenos.h"
#include "IPC/SharedMemoryCantidadOxigenos.h"
#include "Logger.h"


class Oxigeno {
    Semaphore semComienzoHidrogeno;
    Semaphore semComienzoOxigeno;
    Semaphore semFinH1;
    Semaphore semFinH2;
    
    Semaphore semCantHidrogenos;
    Semaphore semCantOxigenos;
    
    SharedMemoryCantidadOxigenos smCantOxigenos;
    SharedMemoryCantidadHidrogenos smCantHidrogenos;
  
    
    Logger logger;
    int nro;
    
    void salir();
    
    void decrementarAtomos();
public:
    Oxigeno(int nro);
    void iniciar();
    void terminar();
    virtual ~Oxigeno();


};

#endif	/* OXIGENO_H */

