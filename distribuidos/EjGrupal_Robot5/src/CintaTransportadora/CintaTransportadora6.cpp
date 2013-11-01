/* 
 * File:   CintaTransportadoraInicial.cpp
 * Author: knoppix
 * 
 * Created on October 31, 2013, 9:48 PM
 */

#include "CintaTransportadora6.h"

CintaTransportadora6::CintaTransportadora6(int idCinta) {
    this->idCinta = idCinta;
}

CintaTransportadora6::CintaTransportadora6() {
}

CintaTransportadora6::CintaTransportadora6(const CintaTransportadora6& orig) {
}

CintaTransportadora6::~CintaTransportadora6() {
}

void CintaTransportadora6::iniciarCinta(int idClaveMemoria, int idClaveSemaforo) {
    
    semaforoAcceso = Semaphore();
    semaforoAcceso.getSemaphore(DIRECTORY, idClaveSemaforo, 1);
    
    cinta = CintaTransportadoraSharedMemory();
    cinta.getSharedMemory(DIRECTORY, idClaveMemoria);

}

void CintaTransportadora6::depositarProductoEnProduccion(ProductoEnProduccion producto) {
    /* Tomo la memoria compartida de la cinta.
     */
    semaforoAcceso.wait();
    {
        /* Leo el contenido actual de la cinta.
         */
        CintaTransportadora_6 *cintaTransportadora = cinta.readInfo();
        
        /* Deposito el gabinete en la posicion actual e indico que ese lugar no esta mas vacio
         */
        (*cintaTransportadora).productoProduccion[(*cintaTransportadora).posicionActual] = producto;
        (*cintaTransportadora).lugarVacio[(*cintaTransportadora).posicionActual] = false;
        
        mostrarEstadoCinta((*cintaTransportadora));
        
        /* Escribo el nuevo valor en la memoria compartida.
         */
        cinta.writeInfo(cintaTransportadora);
    }
    semaforoAcceso.signal();
}

EstadoCinta CintaTransportadora6::obtenerEstadoCinta() {
    EstadoCinta estado;
    estado.cantOcupados = 0;
    
    /* Tomo la memoria compartida de la cinta.
     */
    semaforoAcceso.wait();
    {
        /* Leo el contenido actual de la cinta.
         */
        CintaTransportadora_6 *cintaTransportadora = cinta.readInfo();
        
        estado.ocupado = ! (*cintaTransportadora).lugarVacio[(*cintaTransportadora).posicionActual];
        for (int i = 0; i < TAM_CINTA_INICIAL; ++i) {
            if (! (*cintaTransportadora).lugarVacio[i]) estado.cantOcupados++;
        }
    }
    semaforoAcceso.signal();
    
    return estado;
}

void CintaTransportadora6::mostrarEstadoCinta(CintaTransportadora_6 cintaTransportadora) {
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Estado cinta %d: ", this->idCinta);
    for (int i = 0; i < TAM_CINTA_INICIAL; ++i) {
        if (cintaTransportadora.lugarVacio[i]) {
            strcat(buffer, "|0|");
        }
        else {
            strcat(buffer, "|X|");
        }
    }
    strcat(buffer, "\n");
    write(fileno(stdout),buffer,strlen(buffer));
}