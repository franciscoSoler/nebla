#ifndef CINTA_H
#define	CINTA_H

/* Requisitos que debe poseer la clase PACKAGE para poder ser almacenada
 * dentro de la cinta:
 *   - Debe poseer un constructor por defecto, que debe inicializar al
 *     paquete como vacío
 *   - Debe poseer un método con la firma 'bool estaVacio()' que determine
 *     si el paquete está vacío o no
 *   - No debe utilizar memoria dinámica (esto es requisito para que pueda
 *     se almacenado en una SharedMemory
 */

#include <string.h>
#include <iostream>
#include <Common.h>

// Solo para debugging
#include <Logger/Logger.h>
#include <string>

template <class PACKAGE, unsigned int SIZE>
class Cinta {
    
public:
    Cinta();
    
    /* Pre: La cinta solo puede ser movida si no hay un elemento al final de
     * la misma. 
     * Retorna true si la cinta pudo ser movida.
     */
    bool moverCinta();
    
    /* Retira un paquete del final de la cinta. No puede retirarse un 
     * elemento del final de la cinta si no hay uno. Se debe mover la cinta
     * antes.
     * Retorna true si el paquete pudo ser insertado
     */
    bool retirarPaquete(PACKAGE & paquete);
    
    /* Inserta un elemento al comienzo de la cinta. No puede insertarse un
     * elemento al inicio de la misma si ya hay un elemento en el mismo. 
     * Retorna true si el paquete pudo ser insertado
     */
    bool insertarPaquete(PACKAGE paquete);
    
    unsigned int getCantidadDePaquetes() const;
    
    SerializedCinta serializeData();
    
    void deserializeData(std::stringstream & ss);
    
    /* Impresión en ASCII art de la cinta
     */
    std::string toString();
    
        
private:
    PACKAGE array_[SIZE];
    unsigned int cantidadDePaquetes_;
};


template <class PACKAGE, unsigned int SIZE>
Cinta<PACKAGE, SIZE>::Cinta() {
    cantidadDePaquetes_ = 0;
}

template <class PACKAGE, unsigned int SIZE> 
bool Cinta<PACKAGE, SIZE>::moverCinta() {
    if (array_[SIZE-1].estaVacio()) {
        // FIXME: Mejorar esto, es horrible esta implementación
        PACKAGE auxArray[SIZE];
        
        // Muevo la cinta, literalmente 
        for (unsigned int i = 0; i < SIZE; ++i) {
            auxArray[(i+1) % SIZE] = array_[i];
        }
        
        memcpy(array_, auxArray, sizeof(array_));
        return true;
    }
    
    return false;
}

template <class PACKAGE, unsigned int SIZE>
bool Cinta<PACKAGE, SIZE>::retirarPaquete(PACKAGE & paquete) {
    if (array_[SIZE-1].estaVacio()) {
        return false;
    }
    else {
        paquete = array_[SIZE-1];
        PACKAGE paqueteVacio;
        array_[SIZE-1] = paqueteVacio;
        
        cantidadDePaquetes_--;
        return true;
    }
}

template <class PACKAGE, unsigned int SIZE> 
bool Cinta<PACKAGE, SIZE>::insertarPaquete(PACKAGE paquete) {
    if (array_[0].estaVacio()) {
        array_[0] = paquete;
        cantidadDePaquetes_++;
        
        return true;
    }
    
    return false;
}

template <class PACKAGE, unsigned int SIZE>
std::string Cinta<PACKAGE, SIZE>::toString() {
    std::string cinta = "";
    
    for (unsigned int i = 0; i < SIZE; i++) {
        cinta += "|";
        cinta += array_[i].estaVacio() ? "o" : "x";
    }
    
    cinta += "|";

    char buffer[TAM_BUFFER];
    for (unsigned int i = 0; i < SIZE; ++i) {
        if (!array_[i].estaVacio()) {
            sprintf(buffer, "posCinta: %d, cantCajas: %d, idProd: %d, ODC: %ld",
                    i, cantidadDePaquetes_, array_[i].idProducto_, array_[i].idOrdenDeCompra_);
            Logger::logMessage(Logger::DEBUG, buffer);
        }
    }

    return cinta;
}

template <class PACKAGE, unsigned int SIZE> 
unsigned int Cinta<PACKAGE, SIZE>::getCantidadDePaquetes() const {
    return cantidadDePaquetes_;
}


template <class PACKAGE, unsigned int SIZE> 
SerializedCinta Cinta<PACKAGE, SIZE>::serializeData() {
    SerializedCinta data;
    std::stringstream ss;
    
    for (unsigned int i = 0; i < SIZE; ++i) {
        SerializedCaja dataCinta = array_[i].serializeData();
        ss << dataCinta.data << " ";
    }
    ss << this->cantidadDePaquetes_ << " ";
    strcpy(data.data, ss.str().c_str());
    return data;
}
    
template <class PACKAGE, unsigned int SIZE> 
void Cinta<PACKAGE, SIZE>::deserializeData(std::stringstream & ss) {
    
    for (unsigned int i = 0; i < SIZE; ++i) {
        array_[i].deserializeData(ss);
    }
    ss >> this->cantidadDePaquetes_;
    
}

#endif	/* CINTA_H */

