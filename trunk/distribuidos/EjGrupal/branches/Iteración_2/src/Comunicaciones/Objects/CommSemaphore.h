#ifndef COMM_SEMAPHORE_H_
#define COMM_SEMAPHORE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../Exceptions/Exception.h"
#include "../IPCObject/IPCObject.h"
#include "../../Common.h"

namespace IPC {

class CommSemaphore : public IPCObject
{
private:

	int getId(const char *fileName, int id, int qty, int flags);

	int id;
	// Cantidad de semáforos declarados en el set
	int cantSem;
	// Buffer for output errors
	char buffer_[TAM_BUFFER];
	
public:

	CommSemaphore(std::string IPCName = "");

	virtual ~CommSemaphore();

	void createSemaphore(const char *fileName, int id, int qty);

	void getSemaphore(const char *fileName, int id, int qty);
		
	void initializeSemaphore(int numSem, int val);

	void destroy(void);

	void wait(int numSem = 0);

	void signal(int numSem = 0);
};

}

#endif

