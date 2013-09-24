#ifndef _COMMON_H_
#define _COMMON_H_

#define TAM_BUFFER 255
#define CANT_ANALIZADORES 6
#define CANT_MUESTRAS 5

// Path utilizado para inicializar los IPC a través de la función ftok()
#define DIRECTORY "/home/Duran"

// Ids de los IPC
#define ID_SHMEM_MUESTRAS	1
#define ID_SEM_CONTROL		2
#define ID_SEM_PRODUCTORES	3
#define ID_SEM_ANALIZADORES	4

typedef enum {
	VACIO,
	// No toxicas
	SUSTANCIA_1,
	SUSTANCIA_2,
	SUSTANCIA_3,
	// Toxicas
	SUSTANCIA_A,
	SUSTANCIA_B,
	SUSTANCIA_C
} Sustancia;

typedef struct {
	Sustancia muestra[CANT_MUESTRAS];

	bool estadoMuestras[CANT_ANALIZADORES][CANT_MUESTRAS];
	// Se utiliza para saber en que muestra esta bloqueado cada analizador
	int analizadorBloqueado[CANT_ANALIZADORES];	
	// Se utiliza para saber si un productor se encuentra bloqueado
	bool productorBloqueado[CANT_MUESTRAS];
} Muestras;

#endif
