/* 
 * File:   CintaTransportadoraInicial.cpp
 * Author: knoppix
 * 
 * Created on October 31, 2013, 9:48 PM
 */

#include "CintaTransportadoraInicial.h"

CintaTransportadoraInicial::CintaTransportadoraInicial(int idCinta) {
    this->idCinta = idCinta;
}

CintaTransportadoraInicial::CintaTransportadoraInicial() {
}

CintaTransportadoraInicial::CintaTransportadoraInicial(const CintaTransportadoraInicial& orig) {
}

CintaTransportadoraInicial::~CintaTransportadoraInicial() {
}

void CintaTransportadoraInicial::iniciarCinta(int idClaveMemoria, int idClaveSemaforo) {
    
    semaforoAcceso = Semaphore();
    semaforoAcceso.getSemaphore(DIRECTORY, idClaveSemaforo, 1);
    
    cinta = CintaTransportadoraSharedMemory();
    cinta.getSharedMemory(DIRECTORY, idClaveMemoria);

}

void CintaTransportadoraInicial::depositarProductoEnProduccion(ProductoEnProduccion producto) {
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

EstadoCinta CintaTransportadoraInicial::obtenerEstadoCinta() {
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

void CintaTransportadoraInicial::mostrarEstadoCinta(CintaTransportadora_6 cintaTransportadora) {
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