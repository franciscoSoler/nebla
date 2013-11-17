#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>

class MemoriaCompartida
{
    private:
	int shmId;
	void* ptrDatos;
	key_t clave;
	size_t tamanio;
	int cantidadProcesosAdosados() const;

    public:
	MemoriaCompartida();
	MemoriaCompartida(const std::string& archivo, const char letra, size_t tamanio);
	~MemoriaCompartida();
	
	void* crear();
	void* obtener();
	void liberar();
};

#endif
