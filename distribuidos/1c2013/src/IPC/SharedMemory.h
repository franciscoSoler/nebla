#ifndef SHAREDMEM_H_
#define SHAREDMEM_H_

#define SHM_OK			0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <exception>
#include "../common/Object.h"


template <class T>

class SharedMemory: public Object
{
static const char DEFAULT_LETTER='a';
private:

	int 	m_sharedMemId;
	T*		m_data;
	int 	m_Size;

	int attachedProcessAmmount ();

public:

	/**
	 *
	 */
	SharedMemory (unsigned int size =1);

	/**
	 *
	 */
	~SharedMemory ();

	/**
	 *
	 */
	int create ( std::string archivo,char letra=DEFAULT_LETTER );

	/**
	 *
	 */
	void release ();

	/**
	 *
	 */
	void write ( T data , int pos=0);

	/**
	 *
	 */
	T read (int pos=0);

};

template <class T>

SharedMemory<T> :: SharedMemory (unsigned int size)
{
	m_Size = size;
}

template <class T>

SharedMemory<T> :: ~SharedMemory ()
{
}

template <class T>

int SharedMemory<T> :: create ( std::string archivo,char letra )
{
	// generacion de la clave
	key_t key = ftok ( archivo.data(),letra );

	if ( key == -1 )
		return ERROR_FTOK;
	else
	{
		// creacion de la memoria compartida
		this->m_sharedMemId = shmget ( key,sizeof(T)*m_Size,0644|IPC_CREAT );

		if ( this->m_sharedMemId == -1 )
			return ERROR_SHMGET;
		else
        {
			// attach del bloque de memoria al espacio de direcciones del proceso
			void* tempData = shmat ( this->m_sharedMemId,NULL,0 );

			if ( tempData == (void *) -1 )
				return ERROR_SHMAT;
			else
			{
				this->m_data = (T *) tempData;
				return SHM_OK;
			}
		}
	}
}


template <class T>

void SharedMemory<T> :: release ()
{
	// detach del bloque de memoria
	shmdt ( (void *) this->m_data );

	int procAdosados = this->attachedProcessAmmount ();

	if ( procAdosados == 0 )
		shmctl ( this->m_sharedMemId,IPC_RMID,NULL );

}

template <class T>

void SharedMemory<T> :: write ( T data, int pos)
{
	if(m_data == NULL)
	{
		throw new std::exception;
	}

	this->m_data[pos] = data;
}

template <class T>

T SharedMemory<T> :: read (int pos)
{
	return this->m_data[pos];
}

template <class T>

int SharedMemory<T> :: attachedProcessAmmount ()
{
	shmid_ds status;
	shmctl ( this->m_sharedMemId,IPC_STAT,&status );
	return status.shm_nattch;
}

#endif // SHAREDMEM_H_
