#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_


#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../../Common.h"
#include <middlewareCommon.h>
#include "../../Exceptions/Exception.h"
#include "../../IPCObject/IPCObject.h"
#include <IPCs/IPCTemplate/MsgQueue.h>

namespace IPC {

class AbstractMessageQueue : public IPCObject
{
private:
	int getId(const char *fileName, int id, int flags);

protected:
        long idEmisor;
        TipoAgente idTipoReceptor;
        TipoAgente idTipoAgente;
        int id;
        int idIPC;
        char dirIPC[DIR_FIXED_SIZE];
        char buffer[255];
        
        int enviar(const void *dato, size_t size);
        int recibir(int tipo, void *buffer, size_t size);

public:
	/* Pre: -
 	 * Post: -
 	 */
	AbstractMessageQueue(std::string IPCName = "", long idEmisor = 0, 
                TipoAgente idTipoReceptor = ID_TIPO_VACIO, 
                TipoAgente idTipoAgente = ID_TIPO_VACIO); 
	
	/* Pre: - 
 	 * Post: - 
 	 */
	virtual ~AbstractMessageQueue() = 0; 

	/* 
	 * Metodo utilizado para crear la Cola de Mensajes, si ya 
	 * existe una cola de mensajes con la misma clave el metodo falla.
	 * El launcher es el encargado de crear los ipcs, y por lo tanto de 
	 * invocar este metodo.
	 */
	int createMessageQueue(const char *fileName, int id);

	/* 
	 * Metodo utilizado para obtener una Cola de Mensajes, si no existe
	 * una cola de mensajes con la misma clave el metodo falla.
	 */
	int getMessageQueue(const char *fileName, int id);
	
	/* Pre: -
 	 * Post: -
 	 */
	void destroy(void);
	
	/*virtual int enviar (void * dato) = 0;
	virtual int recibir (int tipo, void * dato) = 0;*/

};

}

#endif


