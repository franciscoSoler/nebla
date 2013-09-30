
#include "AbstractMessageQueue.h"

AbstractMessageQueue::AbstractMessageQueue() {
	
}

AbstractMessageQueue::~AbstractMessageQueue() {
	
}

int AbstractMessageQueue::create(char *fileName, int id) {
	return this->getId(fileName, id, 0666|IPC_CREAT|IPC_EXCL);
}

int AbstractMessageQueue::getMessageQueue(char *fileName, int id) {
	return this->getId(fileName, id, 0666);
}

void AbstractMessageQueue::destroy () {
	msgctl ( this->id,IPC_RMID,NULL );
}

int AbstractMessageQueue::getId(char *fileName, int id, int flags) {
	
	char buffer[255];
			
	key_t clave = ftok (fileName, id);
	if ( clave == -1 ) {	
		strcpy(buffer, "Error en MessageQueue - ftok:");
		strcat(buffer, strerror(errno));	
		strcat(buffer, "\n");
		write(fileno(stdout), buffer, strlen(buffer)); 
		
		return errno;
	}
	
	this->id = msgget ( clave ,flags );
	if ( this->id == -1 ) {
		strcpy(buffer, "Error en MessageQueue - msgget:");
		strcat(buffer, strerror(errno));	
		strcat(buffer, "\n");
		write(fileno(stdout), buffer, strlen(buffer)); 
		return errno;
	}
	
	return 0;
}

/*
 * 	
	template <class T> 
	int enviar (T dato) {
		int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(T)-sizeof(long),0 );
		return resultado;
	}
	
	template <class T> 
	int recibir ( int tipo,T* buffer ) {
		int resultado = msgrcv ( this->id,(void *)buffer,sizeof(T)-sizeof(long),tipo,0 );
		return resultado;
	}
*/

