/* 
 * File:   ProcesoLauncher.cpp
 * Author: distribuidos
 *
 * Created on April 12, 2013, 2:58 AM
 */

#include <cstdlib>
#include "IPC/Semaphore.h"
#include "IPC/SharedMemoryCantidadHidrogenos.h"
#include "IPC/SharedMemoryCantidadOxigenos.h"
#include "IPC/SharedMemoryH1Activo.h"
#include "Identificadores.h"
#include "Logger.h"
#include <sstream>

using namespace std;

/*
 *      Proceso lanzador del ejercicio 11
 */
int main(int argc, char** argv) {
    char buffer[256];

    if (argc != 3) {
        strcpy(buffer, "Launcher: Error: Cantidad de parametros invalida\n Uso:\n ./launcher [CANT HIDROGENOS] [CANT OXIGENOS]\n");
        write(fileno(stderr), buffer, strlen(buffer));
        exit(1);
    }
    Logger logger("Lanzador");

    int cantHidrogenos = 0;
    sscanf(argv[1], "%d", &cantHidrogenos);

    int cantOxigenos = 0;
    sscanf(argv[2], "%d", &cantOxigenos);

    stringstream mensajeLog;
    mensajeLog << "Comenzando con Hidrogenos: " << cantHidrogenos << ", Oxigenos: " << cantOxigenos << " .";
    logger.log(mensajeLog.str());

    logger.log("Creando semaforo semComienzoHidrogeno");
    Semaphore semComienzoHidrogeno;
    semComienzoHidrogeno.createSemaphore(DIRECTORIO, ID_SEM_COMIENZO_HIDROGENO);
    semComienzoHidrogeno.initializeSemaphore(0);

    logger.log("Creando semaforo semComienzoOxigeno");
    Semaphore semComienzoOxigeno;
    semComienzoOxigeno.createSemaphore(DIRECTORIO, ID_SEM_COMIENZO_OXIGENO);
    semComienzoOxigeno.initializeSemaphore(0);

    logger.log("Creando semaforo semFinH1");
    Semaphore semFinH1;
    semFinH1.createSemaphore(DIRECTORIO, ID_SEM_FIN_H1);
    semFinH1.initializeSemaphore(0);

    logger.log("Creando semaforo semFinH2");
    Semaphore semFinH2;
    semFinH2.createSemaphore(DIRECTORIO, ID_SEM_FIN_H2);
    semFinH2.initializeSemaphore(0);

    logger.log("Creando semaforo semH1Activo");
    Semaphore semH1Activo;
    semH1Activo.createSemaphore(DIRECTORIO, ID_SEM_H1ACTIVO);
    semH1Activo.initializeSemaphore(1);

    logger.log("Creando semaforo semCantHidrogenos");
    Semaphore semCantHidrogenos;
    semCantHidrogenos.createSemaphore(DIRECTORIO, ID_SEM_CANT_HIDROGENOS);
    semCantHidrogenos.initializeSemaphore(1);

    logger.log("Creando semaforo semCantOxigenos");
    Semaphore semCantOxigenos;
    semCantOxigenos.createSemaphore(DIRECTORIO, ID_SEM_CANT_OXIGENOS);
    semCantOxigenos.initializeSemaphore(1);
    
    logger.log("Creando semaforo semH1Generado");
    Semaphore semH1Generado;
    semH1Generado.createSemaphore(DIRECTORIO, ID_SEM_H1_GENERADO);
    semH1Generado.initializeSemaphore(0);

    logger.log("Creando memoria compartida smH1Activo");
    SharedMemoryH1Activo smH1Activo;
    smH1Activo.createSharedMemory(DIRECTORIO, ID_SM_H1ACTIVO);
    smH1Activo.escribir(false);

    logger.log("Creando memoria compartida smCantHidrogenos");
    SharedMemoryCantidadHidrogenos smCantHidrogenos;
    smCantHidrogenos.createSharedMemory(DIRECTORIO, ID_SM_CANT_HIDROGENOS);
    smCantHidrogenos.escribir(cantHidrogenos);

    logger.log("Creando memoria compartida smCantOxigenos");
    SharedMemoryCantidadOxigenos smCantOxigenos;
    smCantOxigenos.createSharedMemory(DIRECTORIO, ID_SM_CANT_OXIGENOS);
    smCantOxigenos.escribir(cantOxigenos);

    logger.log("Creando procesos hidrogeno.");
    char nroHidrogeno[256];
    for (int i = 1; i <= cantHidrogenos; i++) {
        pid_t pid = fork();
        sprintf(nroHidrogeno, "%d", i);
        if (pid == 0) {
            execlp("./hidrogeno", "hidrogeno",nroHidrogeno, (char *) 0);
            logger.error("Error al crear proceso hidrogeno.");
            exit(1);
        }
    }
    logger.log("Procesos hidrogeno creados.");
    logger.log("Creando procesos oxigeno.");
    char nroOxigeno[256];
    for (int i = 1; i <= cantOxigenos; i++) {
        pid_t pid = fork();
        sprintf(nroOxigeno, "%d", i);
        if (pid == 0) {
            execlp("./oxigeno", "oxigeno", nroOxigeno, (char *) 0);
            logger.error("Error al crear proceso oxigeno.");
            exit(1);
        }
    }
    logger.log("Procesos oxigeno creados.");
    
    logger.log("Despertando primer hidrogeno.");
    semComienzoHidrogeno.signal();
    
    logger.log("Termino.");
    return 0;
}

