
#include "AbstractMessageQueue.h"

AbstractMessageQueue::AbstractMessageQueue() {	
}

AbstractMessageQueue::~AbstractMessageQueue() {
}

int AbstractMessageQueue::create(const char *fileName, int id) {
	return this->getId(fileName, id, 0666|IPC_CREAT|IPC_EXCL);
}

int AbstractMessageQueue::getMessageQueue(const char *fileName, int id) {
	return this->getId(fileName, id, 0666);
}

void AbstractMessageQueue::destroy () {
	msgctl ( this->id,IPC_RMID,NULL );
}

int AbstractMessageQueue::getId(const char *fileName, int id, int flags) {
			
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {
		sprintf(this->buffer, "AbstractMessageQueue - Fallo la operacion ftok: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
	}
	
	this->id = msgget ( clave ,flags );
	if ( this->id == -1 ) {
		sprintf(this->buffer, "AbstractMessageQueue - Fallo la operacion msgget: %s", strerror(errno));
                throw Exception(std::string(this->buffer));
	}
	return 0;
}