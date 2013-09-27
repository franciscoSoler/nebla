#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "/home"

// Ids de los IPCs (Creo que esto está mal)
#define ID_SEMCONTROL		1
#define ID_SHMEM		2
#define ID_GET_VENDEDOR_1	3
#define ID_COMPRAR_1		4
#define ID_VENDER_1		5
#define ID_GET_VENDEDOR_2	6
#define ID_COMPRAR_2		7
#define ID_VENDER_2		8

typedef struct {
	int numero;
} StartComunicationMessage;

typedef struct {
	long mtype;
	int numero;
} HandShakeMessage;

typedef struct {
	int cantidad;
	float dinero;
} TicketsInfo;

typedef struct {
	long mtype;
	int origen;
	int tipo;
	//bool error;
	TicketsInfo ticketsInfo;
} RequestTicketsMessage;

typedef struct {
	bool resultado;
	int vuelto;
	char mensaje[100]; // Nro de ticket o mensaje de error
} ResultadoCompra;

typedef struct {
	long mtype;
	int origen;
	int tipo;
	ResultadoCompra resultadoCompra;
} ResponseTicketsMessage;

typedef struct {
	int origen;
	int destino;
	int tipo;
	size_t size;
	TicketsInfo ticketsInfo;
} NetworkBuyMessage;

typedef struct {
	int origen;
	int destino;
	int tipo;
	size_t size;
	ResultadoCompra resultadoCompra;
} NetworkSellMessage;

#endif
