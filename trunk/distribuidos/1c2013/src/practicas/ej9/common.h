#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "../../../src"

// Ids de los IPCs 
#define ID_SEMCONTROL		1
#define ID_SHMEM			2
#define ID_SEMPRODUCTORES	3

#endif
