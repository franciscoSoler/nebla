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
#include <middlewareCommon.h>

namespace COMM {

class CommSemaphore : public CommObject
{
public:

    CommSemaphore(std::string CommName = "", long idEmisor = 0, TipoAgente idDuenioSem = ID_TIPO_VACIO, TipoAgente idDuenioSemRemoto = ID_TIPO_VACIO);
    virtual ~CommSemaphore();
    void getSemaphore(const char *fileName, int id, int qty);
    void initializeSemaphore(int numSem, int val);
    void wait(int numSem = 0);
    void signal(int numSem = 0);

protected:
    void initializeQueues(const char *fileName, int id);

protected:
    // Buffer for output errors
    long idEmisor_;
    TipoAgente typeDuenioSem_;
    TipoAgente typeDuenioSemRemoto_;
    char buffer_[TAM_BUFFER];
    IPC::MsgQueue senderMsgQueue_;
    IPC::MsgQueue receiverMsgQueue_;
};

}

#endif

