/* 
 * File:   ProcesoTerminator.cpp
 * Author: distribuidos
 *
 * Created on April 13, 2013, 2:20 PM
 */

#include <cstdlib>
#include "IPC/Semaphore.h"
#include "IPC/SharedMemoryCantidadHidrogenos.h"
#include "IPC/SharedMemoryCantidadOxigenos.h"
#include "IPC/SharedMemoryH1Activo.h"
#include "Logger.h"
#include "Identificadores.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Logger logger ("Terminador");
    logger.log("Comenzando.");
    
    logger.log("Destruyendo semaforo semComienzoHidrogeno");
    Semaphore semComienzoHidrogeno;
    semComienzoHidrogeno.getSemaphore(DIRECTORIO, ID_SEM_COMIENZO_HIDROGENO);
    semComienzoHidrogeno.destroy();

    logger.log("Destruyendo semaforo semComienzoOxigeno");
    Semaphore semComienzoOxigeno;
    semComienzoOxigeno.getSemaphore(DIRECTORIO, ID_SEM_COMIENZO_OXIGENO);
    semComienzoOxigeno.destroy();

    logger.log("Destruyendo semaforo semFinH1");
    Semaphore semFinH1;
    semFinH1.getSemaphore(DIRECTORIO, ID_SEM_FIN_H1);
    semFinH1.destroy();

    logger.log("Destruyendo semaforo semFinH2");
    Semaphore semFinH2;
    semFinH2.getSemaphore(DIRECTORIO, ID_SEM_FIN_H2);
    semFinH2.destroy();

    logger.log("Destruyendo semaforo semH1Activo");
    Semaphore semH1Activo;
    semH1Activo.getSemaphore(DIRECTORIO, ID_SEM_H1ACTIVO);
    semH1Activo.destroy();
    
    logger.log("Destruyendo semaforo semH1Generado");
    Semaphore semH1Generado;
    semH1Generado.getSemaphore(DIRECTORIO, ID_SEM_H1_GENERADO);
    semH1Generado.destroy();

    logger.log("Destruyendo semaforo semCantHidrogenos");
    Semaphore semCantHidrogenos;
    semCantHidrogenos.getSemaphore(DIRECTORIO, ID_SEM_CANT_HIDROGENOS);
    semCantHidrogenos.destroy();

    logger.log("Destruyendo semaforo semCantOxigenos");
    Semaphore semCantOxigenos;
    semCantOxigenos.getSemaphore(DIRECTORIO, ID_SEM_CANT_OXIGENOS);
    semCantOxigenos.destroy();

    logger.log("Destruyendo memoria compartida smH1Activo");
    SharedMemoryH1Activo smH1Activo;
    smH1Activo.getSharedMemory(DIRECTORIO, ID_SM_H1ACTIVO);
    smH1Activo.destroy();

    logger.log("Destruyendo memoria compartida smCantHidrogenos");
    SharedMemoryCantidadHidrogenos smCantHidrogenos;
    smCantHidrogenos.getSharedMemory(DIRECTORIO, ID_SM_CANT_HIDROGENOS);
    smCantHidrogenos.destroy();

    logger.log("Destruyendo memoria compartida smCantOxigenos");
    SharedMemoryCantidadOxigenos smCantOxigenos;
    smCantOxigenos.getSharedMemory(DIRECTORIO, ID_SM_CANT_OXIGENOS);
    smCantOxigenos.destroy();
    
    logger.log("Termino.");
    return 0;
}

