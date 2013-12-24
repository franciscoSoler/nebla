/*
 * File:   Terminator.cpp
 * Author: francisco
 *
 * Created on 29 de octubre de 2013, 22:37
 */
#include <iostream>

#include "Exceptions/Exception.h"
#include "Logger/Logger.h"

#include "IPCs/IPCAbstractos/SharedMemory/NumeradorClientesSharedMemory.h"
#include "IPCs/Semaphore/Semaphore.h"

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("Terminator Numerador:");

        IPC::Semaphore semaforoNumeradorClientes("Semaforo Numerador Clientes");
        semaforoNumeradorClientes.getSemaphore(DIRECTORY_NUMERADOR_CLIENTES, ID_NUMERADOR_CLIENTES, 1);
        semaforoNumeradorClientes.destroy();
    
        IPC::NumeradorClientesSharedMemory numeradorClientesShMem("Numerador Clientes SharedMemory");
        numeradorClientesShMem.getSharedMemory(DIRECTORY_NUMERADOR_CLIENTES, ID_NUMERADOR_CLIENTES);
        numeradorClientesShMem.destroy();
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, e.get_error_description().c_str());
        abort();
    }
}

