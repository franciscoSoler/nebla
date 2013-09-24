#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

// Path utilizado para inicializar los IPC a través de la función ftok()
#define DIRECTORY "/home/Duran"

// Ids de los IPCs
#define ID_SEM_EMBOTELLADORA	1
#define ID_SEM_ETIQUETADORA	2
#define ID_SEM_TAPADORA		3
#define ID_SEM_ALMACENADORA	4

#endif
