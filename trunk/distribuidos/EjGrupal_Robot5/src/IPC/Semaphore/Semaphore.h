#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "../../common.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

class Semaphore
{
private:
	// identificador del semáforo para  crear el mismo
	int id;

	// Cantidad de semáforos declarados en el set
	int cantSem;
	
public:

	Semaphore(); 

	virtual ~Semaphore();

	int createSemaphore(const char *fileName, int id, int qty);

	int getSemaphore(const char *fileName, int id, int qty);
		
	int initializeSemaphore(int numSem, int val);

	int destroy(void);

	void wait(int numSem = 0);

	void signal(int numSem = 0); 

private:

	int getId(const char *fileName, int id, int qty, int flags);

};

#endif

