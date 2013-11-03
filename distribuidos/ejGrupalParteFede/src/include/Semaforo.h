#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>
#include <stdio.h>
#include <errno.h>

class Semaforo 
{
    private:
	key_t clave;
        int id;
        int valorInicial;

        int inicializar();

    public:
	Semaforo();
        Semaforo(const std::string& archivo, char letra, const int valorInicial);
	Semaforo(const std::string& archivo, char letra);
        ~Semaforo();

	int crear();
	int obtener();
        int p();
        int v();
        void destruir();
};

#endif /* SEMAFORO_H_ */
