#include "Semaphore.h"
#include <errno.h>

using namespace std;

Semaphore :: Semaphore ( std::string name, int key,int initialValue, int semAmmount ):
		m_InitialValue(initialValue),
		m_SemAmmount(semAmmount),
		m_SemName(name),
		m_Key(key)
{
	key_t clave = ftok ( m_SemName.c_str(),m_Key);

	if(clave < 0)
	{
//		cout <<errno<<endl;
		throw new std::exception;
	}

	m_Id = semget ( clave,m_SemAmmount,0666 | IPC_CREAT );

	if(m_Id < 0)
		throw new std::exception;
}

Semaphore::~Semaphore()
{
}

void Semaphore :: init ()
{
	semnum init;
	init.val = m_InitialValue;

	init.array = new ushort[m_SemAmmount];
    for(int i = 0; i < m_SemAmmount ; i++)
    	init.array[i] = m_InitialValue;

	if(semctl ( m_Id,0,SETALL,init )<0)
		throw new std::exception;

	delete [] init.array;
}

bool Semaphore :: dec (int pos)
{
	struct sembuf operation;

	operation.sem_num = pos;	// numero de semaforo
	operation.sem_op  = -1;	// restar 1 al semaforo
	operation.sem_flg = SEM_UNDO;

	if(semop ( m_Id,&operation,1 ) <0)
	{
		if(errno!=EIDRM &&errno!=EINTR &&errno!=EINVAL)
		{
			std::cout <<"Semop error"<< errno<<endl;
			throw new std::exception;
		}
		else
			return false;
	}

	return true;
}

bool Semaphore :: inc (int pos)
{
	struct sembuf operation;

	operation.sem_num = pos;	// numero de semaforo
	operation.sem_op  = 1;	// sumar 1 al semaforo
	operation.sem_flg = SEM_UNDO;

	while(semop ( m_Id,&operation,1 ) <0)
	{
		if(errno!=EIDRM &&errno!=EINTR &&errno!=EINVAL)
		{
			std::cout <<"Semop error"<< errno<<endl;
			throw new std::exception;
		}
		else
			return false;
	}

	return true;
}

bool Semaphore ::tryDec(int pos)
{
	bool retVal = true;
	semnum init;
	int result;

	if((result =semctl ( m_Id,pos,GETALL,init )) < 0)
	{
		cout << errno<<endl;
		throw new std::exception;
	}

	//Si el semaforo es 0, entonces no es posible decrementar
	//mas
	if(init.array[0] == 0)
		retVal = false;

	return retVal;
}

void Semaphore :: destroy ()
{
	if(semctl ( m_Id,0,IPC_RMID ) < 0)
	{
		cout <<"Error while deleting semaphore errno:" <<errno<<endl;
		throw new std::exception;
	}
}

int Semaphore ::semAmmount()
{
	return m_SemAmmount;
}
