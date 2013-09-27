/* 
 * File:   Oxigeno.cpp
 * Author: distribuidos
 * 
 * Created on April 12, 2013, 9:45 PM
 */

#include "Oxigeno.h"
#include "Identificadores.h"
#include <errno.h>
#include <sstream>


using namespace std;

Oxigeno::Oxigeno(int nro) {
    this->nro=nro;
    
    stringstream etiqueta;
    etiqueta<<"Oxigeno "<<nro;          
    this->logger.setEtiqueta(etiqueta.str());
    
    if (semComienzoHidrogeno.getSemaphore(DIRECTORIO, ID_SEM_COMIENZO_HIDROGENO) == -1 ||
            semComienzoOxigeno.getSemaphore(DIRECTORIO, ID_SEM_COMIENZO_OXIGENO) == -1 ||
            semFinH1.getSemaphore(DIRECTORIO, ID_SEM_FIN_H1) == -1 ||
            semFinH2.getSemaphore(DIRECTORIO, ID_SEM_FIN_H2) == -1 ||
            semCantHidrogenos.getSemaphore(DIRECTORIO, ID_SEM_CANT_HIDROGENOS) == -1 ||
            semCantOxigenos.getSemaphore(DIRECTORIO, ID_SEM_CANT_OXIGENOS) == -1 ||
            smCantHidrogenos.getSharedMemory(DIRECTORIO, ID_SM_CANT_HIDROGENOS) == -1 ||
            smCantOxigenos.getSharedMemory(DIRECTORIO, ID_SM_CANT_OXIGENOS) == -1  )
        
        salir();


}

void Oxigeno::salir() {
   stringstream mensajeLog;
    
    if (errno == EIDRM){
        logger.log("Termino por falta de IPC.");
        exit(0);
    }   
    else{
        mensajeLog<<"Termino con error ( errno: "<<errno<<" ).";
        logger.error(mensajeLog.str());
        exit(1);
    }
}


Oxigeno::~Oxigeno() {
}

void Oxigeno::decrementarAtomos() {
    logger.log("Decrementando 2 hidrogenos y 1 oxigeno.");
    if(semCantHidrogenos.wait()==-1)
        salir();
    if(semCantOxigenos.wait()==-1)
        salir();
    int oxigenos = smCantOxigenos.leer();
    int hidrogenos = smCantHidrogenos.leer();
    smCantHidrogenos.escribir(hidrogenos - 2);
    smCantOxigenos.escribir(oxigenos - 1);
    if(semCantOxigenos.signal()==-1)
        salir();
    if(semCantHidrogenos.signal()==-1)
        salir();
}


void Oxigeno::iniciar(){
    logger.log("Esperando comienzo.");
    if (semComienzoOxigeno.wait() == -1)
        salir();
}

void Oxigeno::terminar(){
    decrementarAtomos();
    
    logger.log("Molecula de agua generada.");
    
    logger.log("Terminando primer hidrogeno.");
    if(semFinH1.signal()==-1)
        salir();
    logger.log("Terminando segundo hidrogeno.");
    if(semFinH2.signal()==-1)
        salir();
    
    
    
    logger.log("Despertando a un nuevo proceso hidrogeno.");
    if(semComienzoHidrogeno.signal()==-1)
        salir();
    logger.log("Termino.");
}