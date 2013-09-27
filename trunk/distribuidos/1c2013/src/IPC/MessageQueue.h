#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>

template<class T> class MessageQueue
{
private:
    key_t clave;
    int id;

public:
    static const char DEFAULT_LETTER='a';

    /**
     *
     */
    MessageQueue( std::string file, char c =DEFAULT_LETTER);

    /**
     *
     */
    ~MessageQueue();

    /**
     *
     */
    int put( T dato );

    //TODO esta interfaz puede cambiar levemente para contemplar la salida
    //por destruccion de la mq
    int get( int tipo, T* buffer );

    /**
     *
     */
    int destroy();
};

template<class T> MessageQueue<T>::MessageQueue( std::string archivo, char letra )
{
    this->clave = ftok( archivo.c_str(), letra );
    if ( this->clave == -1 )
        perror( "Error en ftok" );

    this->id = msgget( this->clave, 0777 | IPC_CREAT );
    if ( this->id == -1 )
        perror( "Error en msgget" );
}

template<class T> MessageQueue<T>::~MessageQueue()
{
}

template<class T> int MessageQueue<T>::destroy()
{
    int resultado = msgctl( this->id, IPC_RMID, NULL );
    return resultado;
}

template<class T> int MessageQueue<T>::put( T dato )
{
    int result = msgsnd( this->id, ( const void * ) &dato,
            sizeof(T) - sizeof(long), 0 );
    return result;
}

template<class T> int MessageQueue<T>::get( int tipo, T* buffer )
{
    int result = msgrcv( this->id, ( void * ) buffer, sizeof(T) - sizeof(long),
            tipo, 0 );
    return result;
}

#endif /* MESSAGE_QUEUE_H_ */
