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
#include "CommObject.h"
#include "../../Common.h"
#include <IPCs/IPCTemplate/MsgQueue.h>

namespace COMM {

class CommSemaphore : public CommObject
{
private:
    // Buffer for output errors
    TipoAgente idDuenioSemRemoto_;
    char buffer_[TAM_BUFFER];
    IPC::MsgQueue senderMsgQueue_;
    IPC::MsgQueue receiverMsgQueue_;
        
	
public:

    CommSemaphore(std::string CommName = "", TipoAgente idDuenioSemRemoto = ID_TIPO_VACIO);

    virtual ~CommSemaphore();

    void getSemaphore(const char *fileName, int id, int qty);

    void initializeSemaphore(int numSem, int val);

    void wait(int numSem = 0);

    void signal(int numSem = 0);
};

}

#endif

