/* 
 * File:   SemaphoreMutex.cpp
 * Author: francisco
 * 
 * Created on 7 de diciembre de 2013, 13:00
 */

#include "CommSemaphoreMutex.h"
#include "Logger.h"

namespace IPC {

CommSemaphoreMutex::CommSemaphoreMutex(std::string IPCName) : CommSemaphore (IPCName)
{}

CommSemaphoreMutex::~CommSemaphoreMutex() {
}


void CommSemaphoreMutex::wait(int numSem)
{
    
    // enviar por una cola el pedido de la shMem!!!
    CommSemaphore::wait(numSem);
}

}

