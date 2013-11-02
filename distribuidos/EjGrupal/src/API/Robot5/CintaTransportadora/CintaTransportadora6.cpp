/* 
 * File:   CintaTransportadoraInicial.cpp
 * Author: nebla
 * 
 * Created on October 31, 2013, 9:48 PM
 */

#include "CintaTransportadora6.h"

#include "../../../Common.h"

CintaTransportadora6::CintaTransportadora6(int idCinta) {
    this->idCinta = idCinta;
}

CintaTransportadora6::CintaTransportadora6() {
}

CintaTransportadora6::CintaTransportadora6(const CintaTransportadora6& orig) {
}

CintaTransportadora6::~CintaTransportadora6() {
}

void CintaTransportadora6::iniciarCinta(int idClaveMem, int idClaveSem) {
    
    semaforoAcceso = IPC::Semaphore("Cinta Transportadora");
    semaforoAcceso.getSemaphore(DIRECTORY_ROBOT_11, idClaveSem, CANTIDAD_CINTAS_6);
    
    cinta = IPC::CintaTransportadoraSharedMemory("CintaTransportadoraSharedMemory");
    cinta.getSharedMemory(DIRECTORY_ROBOT_11, idClaveMem);
}

void CintaTransportadora6::depositarProductoEnProduccion(ProductoEnProduccion producto) {
    /* Tomo la memoria compartida de la cinta.
     */
    semaforoAcceso.wait(this->idCinta);
    {
        /* Leo el contenido actual de la cinta.
         */
        CintaTransportadora_6 *cintaTransportadora = cinta.readInfo();
        
        /* Deposito el gabinete en la posicion actual e indico que ese lugar no esta mas vacio
         */
         int posicion = (((*cintaTransportadora).puntoLectura + BUFF_SIZE_CINTA_6 - 1) % BUFF_SIZE_CINTA_6);
        (*cintaTransportadora).productoProduccion[posicion] = producto;
        (*cintaTransportadora).lugarVacio[posicion] = false;
        (*cintaTransportadora).cantLibres--;
        mostrarEstadoCinta((*cintaTransportadora));
        
        /* Escribo el nuevo valor en la memoria compartida.
         */
        cinta.writeInfo(cintaTransportadora);
    }
    semaforoAcceso.signal(this->idCinta);
}

EstadoCinta CintaTransportadora6::obtenerEstadoCinta() {
    EstadoCinta estado;
    estado.cantOcupados = 0;
    
    /* Tomo la memoria compartida de la cinta.
     */
    semaforoAcceso.wait(this->idCinta);
    {
        /* Leo el contenido actual de la cinta.
         */
        CintaTransportadora_6 *cintaTransportadora = cinta.readInfo();
        
        int posicion = (((*cintaTransportadora).puntoLectura + BUFF_SIZE_CINTA_6 - 1) % BUFF_SIZE_CINTA_6);
        estado.ocupado = ! (*cintaTransportadora).lugarVacio[posicion];
        estado.cantOcupados = BUFF_SIZE_CINTA_6 - (*cintaTransportadora).cantLibres;
    }
    semaforoAcceso.signal(this->idCinta);
    
    return estado;
}

/* TRUE = Robot 11 bloqueado */
bool CintaTransportadora6::robot11Bloqueado() {
    bool estado = false;
    semaforoAcceso.wait(this->idCinta);
    {
        /* Leo el contenido actual de la cinta.
         */
        CintaTransportadora_6 *cintaTransportadora = cinta.readInfo();
        estado = (*cintaTransportadora).robot11Durmiendo;
    }
    semaforoAcceso.signal(this->idCinta);
    return estado;
}

bool CintaTransportadora6::marcarRobot11Liberado() {
    bool estado = false;
    semaforoAcceso.wait(this->idCinta);
    {
        /* Leo el contenido actual de la cinta.
         */
        CintaTransportadora_6 cintaTransportadora;
        cinta.readInfo(&cintaTransportadora);
        cintaTransportadora.robot11Durmiendo = false;
        cinta.writeInfo(&cintaTransportadora);
    }
    semaforoAcceso.signal(this->idCinta);
    return estado;
}


void CintaTransportadora6::mostrarEstadoCinta(CintaTransportadora_6 cintaTransportadora) {
    char buffer[TAM_BUFFER];
    sprintf(buffer, "Estado cinta %d: ", this->idCinta);
    for (int i = 0; i < BUFF_SIZE_CINTA_6; ++i) {
        if (cintaTransportadora.lugarVacio[i]) {
            strcat(buffer, "|0");
        }
        else {
            strcat(buffer, "|X");
        }
    }
    strcat(buffer, "|\n");
    write(fileno(stdout),buffer,strlen(buffer));
}