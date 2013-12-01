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

#include "../AbstractMessageQueue/AbstractMessageQueue.h"
#include <Logger/Logger.h>

namespace IPC {

class Barrera1112MessageQueue : public AbstractMessageQueue
{      
public:
	Barrera1112MessageQueue(std::string IPCName = "", long idEmisor = 0, TipoAgente idTipoAgente = ID_TIPO_VACIO) 
                : AbstractMessageQueue(IPCName, idEmisor, idTipoAgente) {} 
	
	virtual ~Barrera1112MessageQueue() {}

	int send (long idReceptor, MensajeBarrera dato) {
            MsgAgenteReceptor msg;
            msg.mtype = MSG_MUX;
            msg.idEmisor = this->idEmisor;
            msg.idReceptor = idReceptor;
            msg.idIPCReceptor = this->idIPC;
            strcpy(msg.dirIPCReceptor, this->dirIPC);
            
            
            if ( sizeof(MensajeBarrera) > MSG_QUEUE_FIXED_SIZE ) {
                sprintf(this->buffer, "MsgQueue %s Error - send: Mensaje demasiado largo",
                        getIPCName().c_str());
                Logger::logMessage(Logger::ERROR, this->buffer);
            }

            memcpy(msg.msg, &dato, sizeof(MensajeBarrera));
            this->colaMux->send(msg);
            return 0;
	}
	
	int receive ( int tipo, MensajeBarrera* buffer ) {
            MsgAgenteReceptor msg;
            this->colaMux->recv(tipo, msg);
            memcpy(buffer, msg.msg, sizeof(MensajeBarrera));
            return 0;
	}

};

}

#endif	/* BARRERA1112MESSAGEQUEUE_H */

