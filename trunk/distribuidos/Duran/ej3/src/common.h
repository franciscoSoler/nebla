#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "/home/Duran"

// Ids de los IPCs (Creo que esto está mal)
#define ID_SHMEM		1
#define ID_SEMCONTROL		2

#define ID_COLA_INTERCAMBIO	3
#define ID_COLA_TRANSPORTADORA	4
#define ID_COLA_VERIFICADOS	5
#define ID_COLA_PELIGROSOS	6

#define TIPO_VERIFICACION	1
#define TIPO_SOSPECHOSOS	2

typedef struct {
	int cantidad;
	bool listo;
} InfoVuelo;

typedef struct {
	InfoVuelo vuelos[TAM_BUFFER];
} Informacion;

typedef enum {
	SUSTANCIA_NO_PELIGROSA = 0,
	SUSTANCIA_PELIGROSA_1 = 1,
	SUSTANCIA_PELIGROSA_2 = 2,
	SUSTANCIA_PELIGROSA_3 = 3
} Contenido;

typedef struct {
	int size;
	Contenido contenido;
} Equipaje;

typedef struct {
	long mtype; // Estacion de check In = nroVuelo
	int nroPersona;
	Equipaje equipaje;
} MensajeIntercambioEquipaje;

typedef struct {
	long mtype; // Estacion de verificacion = cte
	MensajeIntercambioEquipaje mensajeEquipajeVerificado;
} MensajeVerificacionEquipaje;

typedef struct {
	long mtype; // Estacion analisis sospechosos = cte
	int vueloDestino;
	Equipaje equipajeSospechoso;
} MensajeEquipajeSospechoso;

#endif
