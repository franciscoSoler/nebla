/* 
 * File:   SemaphoreMutex.cpp
 * Author: francisco
 * 
 * Created on 7 de diciembre de 2013, 13:00
 */

#include "SemaphoreMutex.h"
#include "Logger.h"

namespace IPC {

SemaphoreMutex::SemaphoreMutex(std::string IPCName) : Semaphore (IPCName)
{}

SemaphoreMutex::~SemaphoreMutex() {
}


void SemaphoreMutex::wait(int numSem)
{
    // enviar por una cola el pedido de la shMem!!!
    Semaphore::wait(numSem);
}

}

