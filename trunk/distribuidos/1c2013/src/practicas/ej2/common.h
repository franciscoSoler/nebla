#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "."

// Ids de los IPCs (Creo que esto está mal)
#define ID_SEMCONTROL		1
#define ID_SEMDOOR			2
#define ID_SEMFULL			3 
#define ID_SHMEM				4	
#define BUFF_MAX				7
#define CANT_DOORS			4

// Si bien esta clase solo posee un atributo, puede ser que en un 
// futuro se agregue más
typedef struct
{
	int cantPersonas;
} ControlData;

#endif
