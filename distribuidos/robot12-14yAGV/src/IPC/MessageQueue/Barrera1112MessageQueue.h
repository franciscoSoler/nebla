/* 
 * File:   Barrera1112MessageQueue.h
 * Author: francisco
 *
 * Created on 25 de octubre de 2013, 0:50
 */

#ifndef BARRERA1112MESSAGEQUEUE_H
#define	BARRERA1112MESSAGEQUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "AbstractMessageQueue.h"

class Barrera1112MessageQueue : public AbstractMessageQueue
{

public:
	Barrera1112MessageQueue():AbstractMessageQueue() {} 
	
	virtual ~Barrera1112MessageQueue() {}

	int send (MessageBarrera dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(MessageBarrera)-sizeof(long),0 );
		if (resultado == -1) {
			sprintf(this->buffer, "Barrera1112MessageQueue - Fallo la operacion send: %s", strerror(errno));
                        throw Exception(std::string(this->buffer));
		}
		return resultado;
	}
	
	int receive ( int tipo, MessageBarrera* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(MessageBarrera)-sizeof(long),tipo,0 );
		if (resultado == -1) {
			sprintf(this->buffer, "Barrera1112MessageQueue - Fallo la operacion recv: %s", strerror(errno));
			throw Exception(std::string(this->buffer));
		}
		return resultado;
	}

};

#endif	/* BARRERA1112MESSAGEQUEUE_H */

