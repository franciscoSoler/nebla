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

#define ID_BUFFER_CANASTOS_0 3
#define ID_BUFFER_CANASTOS_1 4
#define ID_BUFFER_CANASTOS_2 5

#define ID_SEM_AGV_1 6
#define ID_SEM_AGV_2 7
#define ID_SEM_AGV_3 8

#define ID_COLA_PEDIDOS_PRODUCCION 9

#define ID_CINTA_6_0 10
#define ID_CINTA_6_1 11

#define ID_SEM_BLOQUEO_ROBOT_5 12
#define ID_SEM_ACCESO_ESTADO_ROBOT_5 13
#define ID_MEM_ESTADO_ROBOT_5 14

#define ID_SEM_BLOQUEO_ROBOT_11 15
#define ID_SEM_ACCESO_ESTADO_ROBOT_11 16

#define ID_MEM_ESTADO_ROBOT_11_0 17
#define ID_MEM_ESTADO_ROBOT_11_1 18

/* Tipos utilizados como identificadores de los mensajes de las colas */

#define TIPO_PEDIDO_ROBOT_5 1

#define TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5 2
#define TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5 3

#define TIPO_PEDIDO_CANASTO 1
#define TIPO_PEDIDO_PRODUCCION 1

#define TAM_CINTA_6 5

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
    int nroOrdenCompra;
    TipoProducto tipo; // Nº de producto
    int cantidad;
    int diferenciaMinima;
} PedidoProduccion;

typedef struct {
    long mtype; // Tipo pedido de produccion
    PedidoProduccion pedidoProduccion;    
} MensajePedidoProduccion;

typedef struct {
    TipoPieza tipoPieza;
    int idAgv;
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
} MensajePedidoAgv;

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
    int idAgv;
    Canasto canasto;
} MensajeRespuestaCanasto;

typedef struct {
    long mtype; // tipo = respuesta de gabinete
    bool ultimo;
    int ordenDeCompra;
    Gabinete gabinete;
} MensajeRespuestaGabinete;

/* Producto en produccion. el cual se deposita en las cintas transportadoras.
 */

typedef struct {
    Gabinete gabinete;  
    int nroOrdenCompra;
    bool falla;
} ProductoEnProduccion;

/* Cinta transportadora */

typedef struct {
    ProductoEnProduccion productoProduccion[TAM_CINTA_6];
    bool lugarVacio[TAM_CINTA_6];
    int posicionActual;
} CintaTransportadora_6;

#endif
