#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CSO/Exceptions/Exception.h"
#include <CSO/IPCs/IPCObject.h>
#include <common.h>

namespace IPC {

class Semaphore : public IPCObject
{
private:

	int getId(const char *fileName, int id, int qty, int flags);

	int id;
	// Cantidad de semáforos declarados en el set
	int cantSem;
	// Buffer for output errors
	char buffer_[200];
	
public:

	Semaphore(std::string IPCName = "");

	virtual ~Semaphore();

	void createSemaphore(const char *fileName, int id, int qty);

	void getSemaphore(const char *fileName, int id, int qty);
		
	void initializeSemaphore(int numSem, int val);

	void destroy(void);

	void wait(int numSem = 0);

	void signal(int numSem = 0);
};

}

#endif

