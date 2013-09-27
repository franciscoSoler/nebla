#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "../../../src"

// Ids de los IPCs (Creo que esto está mal)
#define ID_SEMCONTROL		1
#define ID_SHMEM			2
#define ID_GET_VENDEDOR		3
#define ID_COMPRAR			4
#define ID_VENDER			5

typedef struct {
	long mtype;
	int id;
} HandShakeMessage;

typedef struct {
	int cantidad;
	float dinero;
} TicketsInfo;

typedef struct {
	long mtype;
	TicketsInfo ticketsInfo;
	/*int cantTickets;
	int dinero;*/
} RequestTicketsMessage;

typedef struct {
	bool resultado;
	int vuelto;
	char mensaje[100]; // Nro de ticket o mensaje de error
} ResultadoCompra;

typedef struct {
	long mtype;
	ResultadoCompra resultadoCompra;
	/*bool resultado;
	int vuelto;
	char mensaje[100]; // Nro de ticket o mensaje de error*/
} ResponseTicketsMessage;

#endif
