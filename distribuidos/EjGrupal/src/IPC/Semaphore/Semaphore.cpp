#include "Semaphore.h"

#include "../IPCException.h"

//-----------------------------------------------------------------------------
Semaphore::Semaphore() {
}

//-----------------------------------------------------------------------------
Semaphore::~Semaphore() {
}

//-----------------------------------------------------------------------------
int Semaphore::destroy(void)
{
	int resultado = semctl(this->id, 0, IPC_RMID, (struct semid_ds *) 0);
	if ( resultado == -1 ) {		
		char error[255];
		sprintf(error, "Fallo la operacion semctl (destroy): %s", strerror(errno));
		char className[255];
		strcpy(className, "Semaphore");
		throw IPCException(className, error);
	}
	return resultado;	
}

//-----------------------------------------------------------------------------
int Semaphore::initializeSemaphore(int numSem, int val)
{
	// Structure used in semctl
	union semun {
		int val;					// Value for SETVAL
		struct semid_ds *buf;		// Buffer por IPC_STAT, IPC_SET
		unsigned short *array;		// Array for GETALL, SETALL
		struct semifo *__buf;		// Buffer for IPC_INFO(Linux specific)
	} arg;

	arg.val = val;
	int resultado = semctl(this->id, numSem, SETVAL, arg);
	if ( resultado == -1 ) {		
		char error[255];
		sprintf(error, "Fallo la operacion semctl (init): %s", strerror(errno));
		char className[255];
		strcpy(className, "Semaphore");
		throw IPCException(className, error);
	}
	return resultado;
} 

int Semaphore::getSemaphore(const char *fileName, int id, int qty) {
	int result = this->getId(fileName, id, qty, 0666);
	return result;
}

int Semaphore::createSemaphore(const char *fileName, int id, int qty) {
	int result = this->getId(fileName, id, qty, 0666|IPC_CREAT|IPC_EXCL);
	return result;
}

//-----------------------------------------------------------------------------
int Semaphore::getId(const char *fileName, int id, int qty, int flags)
{ 
	char buffer[255];
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		char error[255];
		sprintf(error, "Fallo la operacion ftok: %s", strerror(errno));
		char className[255];
		strcpy(className, "Semaphore");
		throw IPCException(className, error);
	}

	this->id = semget( clave, qty, flags);
	if ( this->id == -1 ) {	
		char error[255];
		sprintf(error, "Fallo la operacion semget: %s", strerror(errno));
		char className[255];
		strcpy(className, "Semaphore");
		throw IPCException(className, error);
	}
	return 0;
}

//-----------------------------------------------------------------------------
void Semaphore::wait(int numSem)
{	
	struct sembuf oper;
	oper.sem_num = numSem;
	oper.sem_op = -1;
	int resultado = semop(this->id, &oper, 1);
	if (resultado == -1) {
		char error[255];
		sprintf(error, "Fallo la operacion wait: %s", strerror(errno));
		char className[255];
		strcpy(className, "Semaphore");
		throw IPCException(className, error);	
	}
}

void Semaphore::signal(int numSem)
{
	struct sembuf oper;
	oper.sem_num = numSem;
	oper.sem_op = 1;
	oper.sem_flg = 0;
	int resultado = semop(this->id, &oper, 1);
	if (resultado == -1) {
		char error[255];
		sprintf(error, "Fallo la operacion signal: %s", strerror(errno));
		char className[255];
		strcpy(className, "Semaphore");
		throw IPCException(className, error);
	}	
} 	
