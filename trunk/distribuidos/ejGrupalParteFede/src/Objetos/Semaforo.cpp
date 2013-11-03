#include "Semaforo.h"
#include <unistd.h>
#include <stdlib.h>

Semaforo::Semaforo() {
}

Semaforo::Semaforo(const std::string& archivo, char letra, const int valorInicial) : valorInicial(valorInicial)
{
    this->clave = ftok(archivo.c_str(), letra);
}

Semaforo::Semaforo(const std::string& archivo, char letra)
{
    this->clave = ftok(archivo.c_str(), letra);
    if (this->clave == -1)
    {
        perror("Error en ftok al crear semáforo.");
    }
}

Semaforo::~Semaforo() { }

int Semaforo::crear()
{
    this->id = semget(clave, 1, IPC_CREAT | IPC_EXCL | 0660);
    if(this->id == -1)
    {
        perror("Error al crear semáforo.");
    }
    this->inicializar();
    return this->id;
}

int Semaforo::obtener()
{
    this->id = semget(clave, 1, 0660);
    if(this->id == -1)
    {
        perror("Error al obtener semáforo.");
    }
    return this->id;
}

int Semaforo::inicializar()
{
    union semnum
    {
	int val;
	struct semid_ds* buf;
	ushort* array;
    } init;

    init.val = this->valorInicial;
    int resultado = semctl(this->id, 0, SETVAL, init);
    return resultado;
}

int Semaforo::p()
{
    struct sembuf operacion;

    operacion.sem_num = 0;
    operacion.sem_op = -1;
    operacion.sem_flg = 0;

    int resultado = semop(this->id, &operacion, 1);
    return resultado;
}

int Semaforo::v()
{
    struct sembuf operacion;

    operacion.sem_num = 0;
    operacion.sem_op = 1;
    operacion.sem_flg = 0;

    int resultado = semop(this->id, &operacion, 1);
    return resultado;
}

void Semaforo::destruir()
{
    this->obtener();
    semctl(this->id, 0, IPC_RMID);
}