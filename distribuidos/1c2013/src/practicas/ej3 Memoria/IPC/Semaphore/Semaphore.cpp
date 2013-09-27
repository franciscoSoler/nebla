#include "Semaphore.h"

//-----------------------------------------------------------------------------
Semaphore::Semaphore() {
}

//-----------------------------------------------------------------------------
Semaphore::~Semaphore() {
}

//-----------------------------------------------------------------------------
int Semaphore::destroy(void)
{
	return (semctl(this->id, 0, IPC_RMID, (struct semid_ds *) 0));	
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
	return (semctl(this->id, numSem, SETVAL, arg));
} 

int Semaphore::getSemaphore(char *fileName, int id, int qty) {
	int result = this->getId(fileName, id, qty, 0666);
	return result;
}

int Semaphore::createSemaphore(char *fileName, int id, int qty) {
	int result = this->getId(fileName, id, qty, 0666|IPC_CREAT|IPC_EXCL);
	return result;
}

//-----------------------------------------------------------------------------
int Semaphore::getId(char *fileName, int id, int qty, int flags)
{ 
	char buffer[255];
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		strcpy(buffer, "Error en Semaphore - ftok:");
		strcat(buffer, strerror(errno));	
		strcat(buffer, "\n");
		write(fileno(stdout), buffer, strlen(buffer)); 
		return -1;
	}

	this->id =  semget( clave, qty, flags);
	if ( this->id == -1 ) {
		strcpy(buffer, "Error en Semaphore - semget:");
		strcat(buffer, strerror(errno));
		strcat(buffer, "\n");
		write(fileno(stdout), buffer, strlen(buffer)); 
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
int Semaphore::wait(int numSem)
{	
	struct sembuf oper;
	oper.sem_num = numSem;
	oper.sem_op = -1;
	//oper.sem_flg = SEM_UNDO; /* If someone kills the process, the SO undo the operation done by him */
	return (semop(this->id, &oper, 1));
}

int Semaphore::signal(int numSem)
{
	struct sembuf oper;
	oper.sem_num = numSem;
	oper.sem_op = 1;
	oper.sem_flg = 0;
	return (semop(this->id, &oper, 1));
} 	

//-----------------------------------------------------------------------------
/*int Semaphore::getSemVal(int numSem)
{
	union semun 
	{
		int val;								// Value for SETVAL
		struct semid_ds *buf;		// Buffer por IPC_STAT, IPC_SET
		unsigned short *array;	// Array for GETALL, SETALL
		struct semifo *__buf;		// Buffer for IPC_INFO(Linux specific)
	} arg;

	return semctl(this->semid, numSem, GETVAL, arg);
}*/	
