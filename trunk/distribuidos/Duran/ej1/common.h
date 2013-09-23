#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "/home/distribuidos/Distribuidos/Duran"

// Ids de los IPCs (Creo que esto está mal)
#define ID_SEMCONTROL		1
#define ID_SHMEM		2

typedef struct {
	bool abierto;
	int  cantidad;
	int  total;
	int  cantidadMaxima;
} InfoMuseo;

#endif
