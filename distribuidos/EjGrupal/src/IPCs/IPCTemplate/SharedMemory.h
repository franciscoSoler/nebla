#ifndef SHAREDMEMORY_H
#define	SHAREDMEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <Exceptions/Exception.h>
#include <IPCs/IPCObject/IPCObject.h>

namespace IPC {

template< class T, unsigned int SIZE = sizeof(T) >
class SharedMemory : public IPCObject
{
public:
	SharedMemory(std::string IPCName = "") :  IPCObject(IPCName)
                                            , id(0)
                                            , data_(NULL)
                                            , size_(SIZE)

    {}
  
	/* Pre:
 	 * Post:
 	 */
	virtual ~SharedMemory() {};

	/* 
	 * Metodo utilizado para obtener una Memoria Compartida, si no existe
	 * una cola de mensajes con la misma clave el metodo falla.
	 */
	void getSharedMemory(const char *fileName, int id) {
        if (this->getId(fileName, id, 0666) == -1) {
            sprintf(buffer_, "Shared Memory %s Error - getSharedMemory: %s",
                    getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }

        try {
            attachMemory();
        }
        catch (Exception & e) {
            throw e;
        }
    }
	
	/* 
	 * Metodo utilizado para crear la Memoria Compartida, si ya 
	 * existe una memoria compartida con la misma clave el metodo falla.
	 * El launcher es el encargado de crear los ipcs, y por lo tanto de 
	 * invocar este metodo.
	 */
	void createSharedMemory(const char *fileName, int id) {
        if (this->getId(fileName, id, 0666|IPC_CREAT|IPC_EXCL) == -1) {
            sprintf(buffer_, "Shared Memory %s Error - createSharedMemory: %s",
                    getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }

        try {
            attachMemory();
        }
        catch (Exception & e) {
            throw e;
        }     
    }
	
	/* Método que destruye a la memoria compartida en su totalidad. Desadosa
	 * a todos los procesos que estaban en la misma y luego lo elimina.
	 * La idea es que este método solamente sea utilizado por el proceso
	 * Terminator
	 */
	void destroy(void) {
      	while (this->getCantAttachProcesses()) {
            // Me desattacho de la memoria
            if (shmdt ( (void *)data_ ) != 0) {
                break;
            }
        }

        if (shmctl(this->id, IPC_RMID, NULL) != 0) {
            sprintf(buffer_, "SharedMemory %s Error - destroy: %s",
            getIPCName().c_str(), strerror(errno));
            throw Exception(std::string(buffer_));
        }  
    }
    
    void read(T* data) const {
        memcpy(data, data_, size_);
    }
    
    void write(T* data) {
        memcpy(data_, data, size_);
    }
    
private:
	int getCantAttachProcesses(void) const {
        shmid_ds estado;
        shmctl(this->id, IPC_STAT, &estado);
        return estado.shm_nattch;
    }
    
	void attachMemory() {
        // Me attacho a la memoria dejando al SO que elija donde ubicar la memoria 
        //(atributo en NULL)y para lectura/escritura (atributo en 0)
        void *shmaddr = shmat(this->id, NULL, 0);
        if (shmaddr == (void *)-1)
        {
            sprintf(buffer_, "Shared Memory %s Error - shmat: %s\n",
            getIPCName().c_str(), strerror(errno));
            throw Exception(buffer_);
        }

        data_ = shmaddr;
    }
    
	int getId(const char *fileName, int id, int flags) {
        key_t clave = ftok (fileName, id);
        if ( clave == -1 ) {
            return -1;
        }

        this->id =  shmget( clave, size_ , flags);
        if ( this->id == -1 ) {
            return -1;
        }
        return 0;
    }

	int id;
    void* data_;
    unsigned int size_;
    char buffer_[200];
	
};

} /* namespace IPC */

#endif	/* SHAREDMEMORY_H */

