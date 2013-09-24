#ifndef _COMMON_H_
#define _COMMON_H_

#define TAM_BUFFER 255

// Path utilizado para inicializar los IPC a través de la función ftok()
#define DIRECTORY "/home/Duran"

// Ids de los IPC
#define ID_SHMEM		1
#define ID_SEMCONTROL		2
#define ID_SEMPUERTAS		3
#define ID_COLA_ENTRADA		4
#define ID_COLA_RESPUESTA	5
#define ID_COLA_SALIDA		6

typedef struct {
	bool abierto;
	int  cantidad;
	int  cantidadMaxima;
	bool puertasBloqueadas[TAM_BUFFER];
} InfoMuseo;

typedef struct {
	long mtype;
	bool resultado;
} MensajeRespuesta;

typedef struct {
	long mtype;
	int idPersona;
} MensajeSalida;

typedef struct {
	long mtype;
	int idPersona;
} MensajeEntrada;

#endif
