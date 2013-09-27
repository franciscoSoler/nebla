/* 
 * File:   SharedMemoryCantidadHidrogenos.cpp
 * Author: distribuidos
 * 
 * Created on April 12, 2013, 2:25 AM
 */

#include "SharedMemoryCantidadHidrogenos.h"

SharedMemoryCantidadHidrogenos::SharedMemoryCantidadHidrogenos() {
}

void SharedMemoryCantidadHidrogenos::escribir(int cantHidrogenos){
   (* (int*)this->data)=cantHidrogenos;
}

int SharedMemoryCantidadHidrogenos::leer(){
    return *((int*)this->data);
}

SharedMemoryCantidadHidrogenos::~SharedMemoryCantidadHidrogenos() {
}

size_t SharedMemoryCantidadHidrogenos::getMemorySize(){
    return sizeof(int);
}
