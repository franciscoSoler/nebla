#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

#define DIRECTORY "/home/Duran"

/* Id de los IPC utilizados */

#define ID_COLA_PEDIDOS_AGV_5 1
#define ID_SEM_SINCRONIZACION_5 2

#define ID_MEM_BUFFER_CANASTOS_1 3
#define ID_MEM_BUFFER_CANASTOS_2 4
#define ID_MEM_BUFFER_CANASTOS_3 5

#define ID_SEM_ACCESO_BUFFER_CANASTOS_1 6
#define ID_SEM_ACCESO_BUFFER_CANASTOS_2 7
#define ID_SEM_ACCESO_BUFFER_CANASTOS_3 8

#define ID_SEM_AGV_1 9
#define ID_SEM_AGV_2 10
#define ID_SEM_AGV_3 11

/* Tipos utilizados como identificadores de las colas */

#define TIPO_PEDIDO_CANASTO 1


typedef int IDAgv;

typedef enum {
    PIEZA_1 = 0,
    PIEZA_2 = 1,
    PIEZA_3
} TipoPieza;

typedef struct {
    TipoPieza tipo;
    IDAgv idAgv;
} PedidoCanasto;

typedef struct {
    TipoPieza tipoPieza;
    int cantidadPiezas;
} Canasto;

typedef struct {
	long mtype; // tipo = PEDIDO
	PedidoCanasto pedidoCanasto;
} PedidoAgv;

#endif
