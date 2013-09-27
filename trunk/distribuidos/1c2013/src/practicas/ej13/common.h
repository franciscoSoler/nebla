#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "../../../src"

// IDs utilizado por los IPCs
#define ID_SEMCONTROL				1
#define ID_SHMEM						2
#define ID_SEMLEC						3
#define ID_SEMESC						4

// Macros para definir constantes del problema
#define MAX_ACCESOS					5

typedef enum 
{
	LIBRE, 
	LEYENDO, 
	ESCRIBIENDO
} Estado;

typedef struct
{
	Estado estado;
	int cLectoresLeyendo;
	int cLectoresEsperando;
	int cEscritoresEscribiendo;
	int cEscritoresEsperando;
	int cAccesos;
} ControlData; 
#endif
