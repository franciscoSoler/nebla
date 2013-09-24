#ifndef _COMMON_H_
#define _COMMON_H_

#define TAM_BUFFER 255

#define TAM_SALA 5
#define MAX_BUSES 10
#define CAPACIDAD_BUS 6

// Path utilizado para inicializar los IPC a través de la función ftok()
#define DIRECTORY "/home/Duran"

// Ids de los IPC
#define ID_SEM_CONTROL_SALA	1
#define ID_SEM_BUSES		2
#define ID_SEM_PUERTA		3
#define ID_SHMEM_SALA		4
#define ID_COLA_PERSONAS	5

#define TIPO_MENSAJE		1

typedef struct {
	bool puertaBloqueada;
	int idPersonas[TAM_SALA];
	bool busEnSala;
	bool busBloqueado[MAX_BUSES];
} InfoSala;

typedef struct {
	long mtype;
	int idPersona;
} MensajePersona;

#endif
