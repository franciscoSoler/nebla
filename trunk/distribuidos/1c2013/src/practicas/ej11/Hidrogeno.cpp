#include "Hidrogeno.h"
#include "Identificadores.h"
#include <errno.h>
#include <sstream>


using namespace std;

Hidrogeno::Hidrogeno(int nro) {
    this->nro = nro;

    stringstream etiqueta;
    etiqueta << "Hidrogeno " << nro;
    this->logger.setEtiqueta(etiqueta.str());

    if (semComienzoHidrogeno.getSemaphore(DIRECTORIO, ID_SEM_COMIENZO_HIDROGENO) == -1 ||
            semFinH1.getSemaphore(DIRECTORIO, ID_SEM_FIN_H1) == -1 ||
            semFinH2.getSemaphore(DIRECTORIO, ID_SEM_FIN_H2) == -1 ||
            semH1Activo.getSemaphore(DIRECTORIO, ID_SEM_H1ACTIVO) == -1 ||
            semCantHidrogenos.getSemaphore(DIRECTORIO, ID_SEM_CANT_HIDROGENOS) == -1 ||
            semComienzoOxigeno.getSemaphore(DIRECTORIO, ID_SEM_COMIENZO_OXIGENO) == -1 ||
            semH1Generado.getSemaphore(DIRECTORIO, ID_SEM_H1_GENERADO) == -1 ||
            smH1Activo.getSharedMemory(DIRECTORIO, ID_SM_H1ACTIVO) == -1 ||
            smCantHidrogenos.getSharedMemory(DIRECTORIO, ID_SM_CANT_HIDROGENOS) == -1 ||
            semCantOxigenos.getSemaphore(DIRECTORIO, ID_SEM_CANT_OXIGENOS) == -1 ||
            smCantOxigenos.getSharedMemory(DIRECTORIO, ID_SM_CANT_OXIGENOS) == -1)
        salir();


}

void Hidrogeno::salir() {
    stringstream mensajeLog;

    if (errno == EIDRM) {
        logger.log("Termino por falta de IPC.");
        exit(0);
    }
    else {
        mensajeLog << "Termino con error ( errno: " << errno << " ).";
        logger.error(mensajeLog.str());
        exit(1);
    }

}

bool Hidrogeno::soyH1() {
    if (semH1Activo.wait() == -1)
        salir();
    bool h1Activo = smH1Activo.leer();

    smH1Activo.escribir(!h1Activo);
    
    if (semH1Activo.signal() == -1)
        salir();
    
    return !h1Activo;

}


bool Hidrogeno::hayAtomos() {
    stringstream mensajeLog;
    if(semCantHidrogenos.wait()==-1)
        salir();
    if(semCantOxigenos.wait()==-1)
        salir();
    int oxigenos = smCantOxigenos.leer();
    int hidrogenos = smCantHidrogenos.leer();
    if(semCantOxigenos.signal()==-1)
        salir();
    if(semCantHidrogenos.signal()==-1)
        salir();
    if (hidrogenos >= 2 && oxigenos >= 1) {
        mensajeLog << "Hay atomos suficientes ( H = " << hidrogenos << " , O = " << oxigenos << " ).";
        logger.log(mensajeLog.str());
        return true;
    }
    mensajeLog << "No hay atomos suficientes ( H = " << hidrogenos << " , O = " << oxigenos << " ).";
    logger.log(mensajeLog.str());
    return false;
}


void Hidrogeno::iniciar(){
    do {
        logger.log("Esperando comienzo.");
        if (semComienzoHidrogeno.wait() == -1)
            salir();
        logger.log("Comenzando.");
    } while (!hayAtomos());
    
    soyH1_=soyH1();
    if (soyH1_) {
        logger.log("Soy el primer hidrogeno de la molecula.");
        logger.log("Despertando segundo hidrogeno.");
        if (semComienzoHidrogeno.signal() == -1)
            salir();
    }else{
        logger.log("Soy el segundo hidrogeno de la molecula.");
    }
    
}
void Hidrogeno::terminar(){
    if(soyH1_){
        if(semH1Generado.signal()==-1)
            salir();
        logger.log("Esperando fin.");
        if (semFinH1.wait() == -1)
            salir();
    }else{
        if(semH1Generado.wait()==-1)
                salir();
        logger.log("Despertando a un proceso oxigeno.");
        semComienzoOxigeno.signal();
        logger.log("Esperando fin.");
        if (semFinH2.wait() == -1)
            salir();
    }
    logger.log("Termino.");
}