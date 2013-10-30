#ifndef _COMMON_H_
#define _COMMON_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TAM_BUFFER 255

#define DIRECTORY "/home/Duran"

/* Id de los IPC utilizados */
#define ID_COLA_API_ROBOT_5 1

#define ID_COLA_PEDIDOS_AGV_5 2

#define ID_MEM_BUFFER_CANASTOS_1 3
#define ID_MEM_BUFFER_CANASTOS_2 4
#define ID_MEM_BUFFER_CANASTOS_3 5

#define ID_SEM_ACCESO_BUFFER_CANASTOS_1 6
#define ID_SEM_ACCESO_BUFFER_CANASTOS_2 7
#define ID_SEM_ACCESO_BUFFER_CANASTOS_3 8

#define ID_SEM_AGV_1 9
#define ID_SEM_AGV_2 10
#define ID_SEM_AGV_3 11



/* Tipos utilizados como identificadores de los mensajes de las colas */

#define TIPO_PEDIDO_ROBOT_5 1

#define TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5 2
#define TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5 3

#define TIPO_PEDIDO_CANASTO 4

/* Estructuras utilizadas para la comunicacion entre los AGV y el robot 5.
 */

typedef int IDAgv;

typedef enum {
    PIEZA_1 = 0,
    PIEZA_2 = 1,
    PIEZA_3
} TipoPieza;

typedef enum {
    GABINETE_1 = 0,
    GABINETE_2 = 1,
    GABINETE_3
} TipoGabinete;

typedef enum {
    PRODUCTO_1 = 0,
    PRODUCTO_2 = 1,
    PRODUCTO_3
} TipoProducto;

typedef enum {
    PEDIDO_PRODUCCION = 0,
    PEDIDO_GABINETE = 1,
    PEDIDO_CANASTO
} TipoPedidoRobot5;

typedef struct {
    TipoProducto tipo;
    int cantidad;
} PedidoProduccion;

typedef struct {
    TipoPieza tipoPieza;
    IDAgv idAgv;
} PedidoCanasto;

typedef struct {
    TipoPieza tipoPieza;
    int cantidadPiezas;
} Canasto;

typedef struct {
    TipoGabinete tipoGabinete;
} Gabinete;

typedef struct {
	long mtype; // tipo = PEDIDO
	PedidoCanasto pedidoCanasto;
} PedidoAgv;

/* Estructuras exclusivas utilizadas por el robot 5 
 */

typedef struct {
    TipoPedidoRobot5 tipo;
    PedidoProduccion pedidoProduccion;
    PedidoCanasto pedidoCanasto;
} PedidoRobot5;

typedef struct {
    long mtype;
    PedidoRobot5 pedidoRobot5;
} MensajePedidoRobot5;

typedef struct {
    long mtype; // tipo = respuesta de canasto
    IDAgv idAgv;
    Canasto canasto;
} MensajeRespuestaCanasto;

typedef struct {
    long mtype; // tipo = respuesta de gabinete
    bool ultimo;
    Gabinete gabinete;
} MensajeRespuestaGabinete;


/* Estructuras utilizadas para la comunicacion entre los robots 5 y 11.
 */


typedef struct {
    int nroOrdenCompra;
    TipoProducto tipo; // Nº de producto
    int cantidad;
    int diferenciaMinima;
} PedidoProducto;

typedef struct {
    Gabinete gabinete;  
    int nroOrdenCompra;
} ProductoEnProduccion;

/* Cinta transportadora */

typedef struct {
    ProductoEnProduccion gabinetes[5];
    bool lugarVacion[5];
    int posicionActual;
} CintaTransportadora_6;

#endif
