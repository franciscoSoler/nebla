/* 
 * File:   SharedMemoryCantidadOxigenos.cpp
 * Author: distribuidos
 * 
 * Created on April 12, 2013, 9:49 PM
 */

#include "SharedMemoryCantidadOxigenos.h"

SharedMemoryCantidadOxigenos::SharedMemoryCantidadOxigenos() {
}



SharedMemoryCantidadOxigenos::~SharedMemoryCantidadOxigenos() {
}

void SharedMemoryCantidadOxigenos::escribir(int cantOxigenos){
    (*(int*)this->data)=cantOxigenos;
}

int SharedMemoryCantidadOxigenos::leer(){
    return *((int*)this->data);
}

size_t SharedMemoryCantidadOxigenos::getMemorySize(){
    return sizeof(int);
}