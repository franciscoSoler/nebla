/* 
 * File:   SharedMemoryH1Activo.cpp
 * Author: distribuidos
 * 
 * Created on April 12, 2013, 2:09 AM
 */

#include "SharedMemoryH1Activo.h"

SharedMemoryH1Activo::SharedMemoryH1Activo()
{
}

void SharedMemoryH1Activo::escribir(bool h1Activo){
     if(h1Activo)
	* ((char*)this->data) = 1;
    else
        * ((char*)this->data) = 0;
}
bool SharedMemoryH1Activo::leer(){
    if( *((char*)this->data) == 0)
           return false;
        return true;
}
size_t SharedMemoryH1Activo::getMemorySize(){
     return sizeof(char);
}
    
SharedMemoryH1Activo::~SharedMemoryH1Activo() {
   
}

