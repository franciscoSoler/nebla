#include "Semaphore.h"
#include <Logger/Logger.h>

namespace IPC {

Semaphore::Semaphore(std::string IPCName) : IPCObject(IPCName),
                                            id(0),
                                            cantSem(1)
                                            
{
}

Semaphore::~Semaphore() 
{
}

void Semaphore::destroy(void)
{
	if (semctl(this->id, 0, IPC_RMID, (struct semid_ds *) 0) == -1) {
        sprintf(buffer_, "Semaphore %s Warning - destroy: %s",
				getIPCName().c_str(), strerror(errno));
        Logger::logMessage(Logger::WARNING, buffer_);
	}
}

void Semaphore::initializeSemaphore(int numSem, int val)
{
	// Structure used in semctl
	union semun {
		int val;					// Value for SETVAL
		struct semid_ds *buf;		// Buffer por IPC_STAT, IPC_SET
		unsigned short *array;		// Array for GETALL, SETALL
		struct seminfo *__buf;		// Buffer for IPC_INFO(Linux specific)
	} arg;

	arg.val = val;

	if (semctl(this->id, numSem, SETVAL, arg) == -1) {
		sprintf(buffer_, "Semaphore %s Error - initializeSemaphore: %s",
				getIPCName().c_str(), strerror(errno));
		throw Exception(std::string(buffer_));
	}
} 

void Semaphore::getSemaphore(const char *fileName, int id, int qty) {
	if (this->getId(fileName, id, qty, 0666) == -1) {
        sprintf(buffer_, "Semaphore %s Warning - getSemaphore: %s",
				getIPCName().c_str(), strerror(errno));
        Logger::logMessage(Logger::WARNING, buffer_);
    }
}

void Semaphore::createSemaphore(const char *fileName, int id, int qty) {
	if (this->getId(fileName, id, qty, 0666|IPC_CREAT|IPC_EXCL) == -1) {
        sprintf(buffer_, "Semaphore %s Warning - createSemaphore: %s",
				getIPCName().c_str(), strerror(errno));
        Logger::logMessage(Logger::WARNING, buffer_);
	}
}

//-----------------------------------------------------------------------------
int Semaphore::getId(const char *fileName, int id, int qty, int flags)
{ 
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		return -1;
	}

	this->id =  semget( clave, qty, flags);
	if ( this->id == -1 ) {
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
void Semaphore::wait(int numSem)
{
    struct sembuf oper;
    oper.sem_num = numSem;
    oper.sem_op = -1;
    oper.sem_flg = 0;

    if (semop(this->id, &oper, 1) == -1) {
            sprintf(buffer_, "Semaphore %s Error - wait: %s",
                            getIPCName().c_str(), strerror(errno));
            throw Exception(std::string(buffer_));
    }
}

void Semaphore::signal(int numSem)
{
    struct sembuf oper;
    oper.sem_num = numSem;
    oper.sem_op = 1;
    oper.sem_flg = 0;

    if (semop(this->id, &oper, 1) == -1) {
            sprintf(buffer_, "Semaphore %s Error - signal: %s",
                            getIPCName().c_str(), strerror(errno));
            throw Exception(std::string(buffer_));
    }
} 	

}
