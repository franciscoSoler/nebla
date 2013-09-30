#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

#define DIRECTORY "/home/Duran"

#define ID_SEMCONTROL		1
#define ID_SHMEM		2

typedef struct {
	bool abierto;
	int  cantidad;
	int  total;
	int  cantidadMaxima;
} InfoMuseo;

#endif
