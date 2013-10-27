#include <iostream>
#include <CSO/IPCs/Semaphore.h>
#include <CSO/IPCs/SharedMemory.h>
#include <CSO/Logger/Logger.h>
#include <Objects/DataSM_R11_R14.h>
#include <Objects/DataSM_R14_R16.h>
#include <CSO/Exceptions/Exception.h>
#include <common.h>

static char buffer[255];
static char param1[20];
static char param2[20];

void createIPCs();
void createProcess(std::string processName, int amountOfProcesses = 1);

int main(int argc, char* argv[]) {
    try {
        createIPCs();
        createProcess("Robot11", 2);
        createProcess("Robot14");
        createProcess("Robot16");
    }
    catch (Exception & e) {
        Logger::getInstance().logMessage(Logger::ERROR, 
        e.get_error_description().c_str());
        return -1;
    }

    return 0;
}


void createIPCs() {
	Logger::getInstance().createLog();
	Logger::getInstance().setProcessInformation("Launcher:");
    
    DataSM_R11_R14 dataSM_R11_R14;
    DataSM_R14_R16 dataSM_R14_R16;
    
	// Create and initialize ShMem
	IPC::SharedMemory<DataSM_R11_R14> SM_R11_R14("SM_R11_R14");
	SM_R11_R14.createSharedMemory(DIRECTORY, SM_R11_R14_ID);
    SM_R11_R14.write( & dataSM_R11_R14 );
    
    IPC::SharedMemory<DataSM_R14_R16> SM_R14_R16("SM_R14_R16");
	SM_R14_R16.createSharedMemory(DIRECTORY, SM_R14_R16_ID);
    SM_R14_R16.write( & dataSM_R14_R16 );
   
	// Do the same with the semaphores
	IPC::Semaphore semMutexSM_R11_R14("semMutexSM_R11_R14");
	semMutexSM_R11_R14.createSemaphore(DIRECTORY, SEM_MUTEX_SM_R11_R14_ID, 1);
	semMutexSM_R11_R14.initializeSemaphore(0, 1);
    
	IPC::Semaphore semMutexSM_R14_R16("semMutexSM_R14_R16");
	semMutexSM_R14_R16.createSemaphore(DIRECTORY, SEM_MUTEX_SM_R14_R16_ID, 1);
	semMutexSM_R14_R16.initializeSemaphore(0, 1);
    
    IPC::Semaphore semR11_Cinta13("semR11_Cinta13");
	semR11_Cinta13.createSemaphore(DIRECTORY, SEM_R11_CINTA_13, AMOUNT_CINTA_13);
    for (int i = 0; i < AMOUNT_CINTA_13; ++i) {
        semR11_Cinta13.initializeSemaphore(i, 0);    
    }
	
    // TODO: Preguntar si con sólo este semáforo alcanza, o tengo que implementar
    // uno para condición de bloqueo
    IPC::Semaphore semR14("semR14");
	semR14.createSemaphore(DIRECTORY, SEM_R14_ID, 1);
	semR14.initializeSemaphore(0, 0);
    
    IPC::Semaphore semR16("semR16");
	semR16.createSemaphore(DIRECTORY, SEM_R16_ID, 1);
	semR16.initializeSemaphore(0, 0);
}

void createProcess(std::string processName, int amountOfProcesses) {
	pid_t pid;

    for (int i = 0; i < amountOfProcesses; i++) {
        if ((pid = fork()) < 0) {
            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);
        }
        else if (pid == 0) {
            // Child process. Pass the arguments to the process and call exec
            sprintf(param1, "%ld", i+1);
            sprintf(buffer, "./%s", processName.c_str());
            execlp(buffer, processName.c_str(), param1, (char *) 0);

            sprintf(buffer, "%s Error: %s", processName.c_str(), strerror(errno));
            Logger::getInstance().logMessage(Logger::ERROR, buffer);

            return;
        }
    }
}
