#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255
#define TOTAL 100
#define MAX_PROCESADORES 50

#define DIRECTORY "/home/Duran"

#define ID_SHMEM_BUFFER		1
#define ID_SEM_CONTROL_BUFFER	2
#define ID_SEM_PROCESADORES	3

typedef struct {
	int contador;
	int informacion[MAX_PROCESADORES];
} Buffer;

#endif
