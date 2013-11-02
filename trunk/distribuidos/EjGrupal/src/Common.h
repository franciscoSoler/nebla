#ifndef COMMON_H
#define	COMMON_H


#include <string.h>
#include <stdio.h>
#include <errno.h>

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY_AGV 		"./AGV"
#define DIRECTORY_ROBOT_5 	"./Robot5"
#define DIRECTORY_ROBOT_11 	"./Robot11"

//ipcs entre AGV y Robot5 (usar DIRECTORY_AGV)
#define ID_SEM_BLOQUEO_AGV              1   // ID para el semaforo de bloqueo de los AGV
#define ID_SEM_BUFFER_AGV_5            	2	// es el id para el semaforo de acceso al buffer
#define ID_BUFFER_AGV_5_0           	3	// es el id tanto para shMemory como semaforo de acceso
#define ID_BUFFER_AGV_5_1		4
#define ID_BUFFER_AGV_5_2		5
#define ID_COLA_PEDIDOS_AGV_5           6	

//ipcs del Robot5
#define ID_COLA_API_ROBOT_5 			1

//mtype colas
#define TIPO_PEDIDO_ROBOT_5 			1
#define TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5 	2
#define TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5 3
#define TIPO_PEDIDO_CANASTO 			4
#define TIPO_PEDIDO_PRODUCCION 			5


//constantes del sistema
#define TAM_BUFFER 				255
#define BUFF_SIZE_CINTA_6               	5
#define CANTIDAD_CINTAS_6               	2
#define CANTIDAD_AGVS                           3

typedef enum {
    PEDIDO_PRODUCCION = 0,
    PEDIDO_GABINETE = 1,
    PEDIDO_CANASTO
} TipoPedidoRobot5;

typedef enum {
    GABINETE_1 = 0,
    GABINETE_2 = 1,
    GABINETE_3
} TipoGabinete;

typedef enum {
    PIEZA_1 = 0,
    PIEZA_2 = 1,
    PIEZA_3 = 2,
    PANTALLA_1 = 3,
    PANTALLA_2 = 4,
    PANTALLA_3
} TipoPieza;

typedef enum {
    PRODUCTO_1 = 0,
    PRODUCTO_2 = 1,
    PRODUCTO_3
} TipoProducto;


typedef struct {
    TipoPieza tipoPieza;
    int idAgv;
} PedidoCanastoAGV;

typedef struct {
	long mtype; // tipo = PEDIDO
	PedidoCanastoAGV pedidoCanasto;
} MensajePedidoAgv;


typedef struct {
    Gabinete gabinete;  
    int nroOrdenCompra;
    bool falla;
} ProductoEnProduccion;

typedef struct {
    bool robot11Durmiendo;
    int puntoLectura;
    int cantLibres;
    bool lugarVacio[BUFF_SIZE_CINTA_6];
    ProductoEnProduccion productoProduccion[BUFF_SIZE_CINTA_6];
} CintaTransportadora_6;

typedef struct {
    TipoGabinete tipoGabinete;
} Gabinete;

typedef struct {
    TipoPieza tipoPieza;
    int cantidadPiezas;
} Canasto;

typedef struct {
    TipoPieza tipoPieza;
    int idAgv;
} PedidoCanasto;

typedef struct {
	long mtype; // tipo = PEDIDO
	PedidoCanasto pedidoCanasto;
} MensajePedidoAgv_5;

//exclusivo Robot 5
typedef struct {
    int nroOrdenCompra;
    TipoProducto tipo; // Nº de producto
    int cantidad;
    int diferenciaMinima;
} PedidoProduccion;

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

#endif	/* COMMON_H */

