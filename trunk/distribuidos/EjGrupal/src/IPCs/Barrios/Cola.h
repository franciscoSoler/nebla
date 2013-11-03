#ifndef COLA_H_
#define COLA_H_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <iostream>

template <class T> class Cola 
{
    private:
        key_t clave;
        int id;

    public:
	Cola();
        Cola(const std::string& archivo, char letra);
        ~Cola();
	int crear();
	int obtener();
        int enviar(T dato);
        int recibir(long tipo, T* buffer);
        void destruir();
};

template <class T> Cola<T> :: Cola() 
{
}

template <class T> Cola<T> :: Cola(const std::string& archivo, char letra) 
{
    this->clave = ftok(archivo.c_str(), letra);
    if (this->clave == -1)
    {
        perror("Error en ftok al crear cola.");
    }
}

template <class T> int Cola<T> :: crear() 
{
    this->id = msgget(this->clave, 0666 | IPC_EXCL | IPC_CREAT);
    if (this->id == -1)
    {
        perror("Error al crear cola.");
	exit(-1);
    }
    
    return this->id;
}

template <class T> int Cola<T> :: obtener() 
{
    this->id = msgget(this->clave, 0666);
    if (this->id == -1)
    {
        perror("Error al obtener cola.");
    }
    
    return this->id;
}

template <class T> Cola<T> :: ~Cola() { }

template <class T> void Cola<T> :: destruir() 
{
    this->obtener();
    msgctl(this->id, IPC_RMID, NULL);
}

template <class T> int Cola<T> :: enviar(T dato) 
{
    return msgsnd(this->id, (const void *) &dato, sizeof (T) - sizeof (long), 0);
}

template <class T> int Cola<T> :: recibir(long tipo, T* buffer) 
{
    return msgrcv(this->id, (void *) buffer, sizeof (T) - sizeof (long), tipo, 0);
}

#endif /* COLA_H_ */
