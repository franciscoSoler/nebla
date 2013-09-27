#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include "../common/Object.h"
#include <iostream>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

class Semaphore:public Object
{
private:
	union semnum
	{
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	int m_Id;
	int m_InitialValue;
	int m_SemAmmount;
	std::string m_SemName;
	int m_Key;


public:
	/**
	 *
	 */
	Semaphore ( std::string nombre,int key, int valorInicial, int semNum =1);

	/**
	 *
	 */
	virtual ~Semaphore();

	void init ();

	/**
	 *
	 */
	bool dec (int pos=0);

	/**
	 *
	 */
	bool inc (int pos=0);

	/**
	 *
	 */
	bool tryDec(int pos=0);

	/**
	 *
	 */
	void destroy ();

	/**
	 *
	 */
	int semAmmount();
};

#endif /* SEMAFORO_H_ */
