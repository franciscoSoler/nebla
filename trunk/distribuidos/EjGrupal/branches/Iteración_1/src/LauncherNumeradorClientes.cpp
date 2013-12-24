/*
 * LauncherNumeradorClientes.cpp
 * 
 * Copyright 2013 Adrian Duran <nebla@nebla>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <iostream>

#include <sys/stat.h>
#include <sys/types.h>

#include "IPCs/IPCAbstractos/SharedMemory/NumeradorClientesSharedMemory.h"
#include "IPCs/Semaphore/Semaphore.h"

#include "Numerador/commonNumerador.h"
#include "API/Objects/Util.h"
#include "Logger/Logger.h"

void createIPCs();
void createDirectory(std::string directoryName);

int main(int argc, char **argv)
{
    try {
        Util::getInstance();
        
        createDirectory(DIRECTORY_NUMERADOR_CLIENTES);

        createIPCs();

        Util::createProcess("NumeradorClientesServer", 1, 1);

    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR,
        e.get_error_description().c_str());
        return -1;
    }

    return 0;
}

void createDirectory(std::string directoryName) {
    mkdir(directoryName.c_str(), 0777);
}

void createIPCs() {

    Logger::getInstance().setProcessInformation("LauncherNumeradorCliente:");

    IPC::Semaphore semaforoNumeradorClientes("Semaforo Numerador Clientes");
    semaforoNumeradorClientes.createSemaphore(DIRECTORY_NUMERADOR_CLIENTES, ID_NUMERADOR_CLIENTES, 1);
    semaforoNumeradorClientes.initializeSemaphore(0,1);
    
    IPC::NumeradorClientesSharedMemory numeradorClientesShMem("Numerador Clientes SharedMemory");
    numeradorClientesShMem.createSharedMemory(DIRECTORY_NUMERADOR_CLIENTES, ID_NUMERADOR_CLIENTES);
   
    ClientesInfo initialInfo;
    int i = 0;
    for (i = 0; i < MAX_CLIENTES; ++i) {
        initialInfo.idClientes[i] = 0;
    }
    
    semaforoNumeradorClientes.wait();
    {
        numeradorClientesShMem.writeInfo(&initialInfo);
    }
    semaforoNumeradorClientes.signal();
}
