#include <iostream>
#include <common.h>
#include <CSO/Exceptions/Exception.h>
#include <CSO/IPCs/Semaphore.h>
#include <CSO/IPCs/SharedMemory.h>
#include <CSO/Logger/Logger.h>
#include <Objects/DataSM_R11_R14.h>
#include <Objects/DataSM_R14_R16.h>

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().setProcessInformation("Terminator:");

        IPC::SharedMemory<DataSM_R11_R14> SM_R11_R14("SM_R11_R14");
        SM_R11_R14.getSharedMemory(DIRECTORY, SM_R11_R14_ID);
        SM_R11_R14.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_R11_R14 destruido");

        IPC::SharedMemory<DataSM_R14_R16> SM_R14_R16("SM_R14_R16");
        SM_R14_R16.getSharedMemory(DIRECTORY, SM_R14_R16_ID);
        SM_R14_R16.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC shMem_R14_R16 destruido");

        IPC::Semaphore semMutexSM_R11_R14("semMutexSM_R11_R14");
        semMutexSM_R11_R14.getSemaphore(DIRECTORY, SEM_MUTEX_SM_R11_R14_ID, 1);
        semMutexSM_R11_R14.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_R11_R14 destruido");

        IPC::Semaphore semMutexSM_R14_R16("semMutexSM_R14_R16");
        semMutexSM_R14_R16.getSemaphore(DIRECTORY, SEM_MUTEX_SM_R14_R16_ID, 1);
        semMutexSM_R14_R16.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semMutex_shMem_R14_R16 destruido");

        IPC::Semaphore semR11_Cinta13("semR11_Cinta13");
        semR11_Cinta13.getSemaphore(DIRECTORY, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
        semR11_Cinta13.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semR11_Cinta13 destruido");

        IPC::Semaphore semR14("semR14");
        semR14.getSemaphore(DIRECTORY, SEM_R14_ID, 1);
        semR14.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semR14 destruido");
        
        IPC::Semaphore semR16("semR16");
        semR16.getSemaphore(DIRECTORY, SEM_R16_ID, 1);
        semR16.destroy();
        Logger::logMessage(Logger::IMPORTANT, "IPC semR16 destruido");
        
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, 
        e.get_error_description().c_str());
        return -1;
    }
}

