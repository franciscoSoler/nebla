#include "MemoriaCompartida.h"
#include <unistd.h>
#include <stdlib.h>
#include <Logger/Logger.h>

MemoriaCompartida::MemoriaCompartida() {}

MemoriaCompartida::MemoriaCompartida(const std::string& archivo, const char letra, size_t tamanio)
{
    this->tamanio = tamanio;
    this->clave = ftok(archivo.c_str(), letra);
    if (this->clave == -1)
    {
        perror("Error en ftok al crear shmem.");
    }
}

void* MemoriaCompartida::crear()
{
    this->shmId = shmget(clave, tamanio, IPC_CREAT | IPC_EXCL | 0666);
    void* tmpPtr = shmat(this->shmId, NULL, 0);
    if(tmpPtr == (void*) - 1)
    {
    Logger::logMessage(Logger::WARNING, "Error al crear shmem.");
	exit(-1);
    }
    
    return this->ptrDatos;
}

void MemoriaCompartida::liberar()
{
    this->obtener();
    shmdt((void *) this->ptrDatos);

    shmctl(this->shmId, IPC_RMID, NULL);
}

void* MemoriaCompartida::obtener()
{
    this->shmId = shmget(clave, tamanio, 0666);
    void* tmpPtr = shmat(this->shmId, NULL, 0);
    if(tmpPtr == (void*) - 1)
    {
        Logger::logMessage(Logger::WARNING, "Error al obtener shmem.");
    }
    this->ptrDatos = tmpPtr;
    return this->ptrDatos;
}

MemoriaCompartida::~MemoriaCompartida() { }

int MemoriaCompartida::cantidadProcesosAdosados() const
{
    shmid_ds estado;
    shmctl(this->shmId, IPC_STAT, &estado);
    return estado.shm_nattch;
}
