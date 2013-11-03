#ifndef COMMON_H
#define	COMMON_H

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define NOMBRE_ARCHIVO_PRODUCTOS        "info_productos.csv"
// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY_AGV 		"./DAGV"
#define DIRECTORY_ROBOT_5 	"./DRobot5"
#define DIRECTORY_ROBOT_11 	"./DRobot11"
#define DIRECTORY_ROBOT_12 	"./DRobot12"

//ipcs entre AGV y Robot5 (usar DIRECTORY_AGV)
#define ID_SEM_BLOQUEO_AGV              1   // ID para el semaforo de bloqueo de los AGV
#define ID_SEM_BUFFER_AGV_5            	2	// es el id para el semaforo de acceso al buffer
#define ID_BUFFER_AGV_5_0           	3	// es el id tanto para shMemory como semaforo de acceso
#define ID_BUFFER_AGV_5_1		4
#define ID_BUFFER_AGV_5_2		5
#define ID_COLA_PEDIDOS_AGV_5           6

// ipcs entre AGV, robot 11 y robot 12 (usar DIRECTORY_AGV)
#define ID_SEM_BUFFER_CANASTOS          7
#define ID_COLA_PEDIDOS_ROBOTS_AGV      8
#define ID_BUFFER_CANASTOS_0            9
#define ID_BUFFER_CANASTOS_1            10
#define ID_BUFFER_CANASTOS_2            11


//ipcs del Robot5 (usar DIRECTORY_ROBOT_5)
#define ID_COLA_API_ROBOT_5 		1

//ipcs entre Robot5 y almacen de piezas (usar DIRECTORY_ROBOT_5)
#define ID_COLA_PEDIDOS_PRODUCCION      2

//ipcs entre robot 11 y robot 5 (usar DIRECTORY_ROBOT_5)
#define ID_SEM_BLOQUEO_ROBOT_5         3
#define ID_ESTADO_ROBOT_5              4 // Se utiliza el mismo para la sh mem y para el semaforo

// ipcs entre robot 5 y robot 11 (cinta transportadora 6, usar DIRECTORY_ROBOT_11)
#define ID_SEM_CINTA_6                  1
#define ID_CINTA_6_0                    2
#define ID_CINTA_6_1                    3

// ipcs entre robot 5, robot 11 y AGV (usar DIRECTORY_ROBOT_11)
#define ID_SEM_BLOQUEO_ROBOT_11         4

// ipcs entre robot 12 y AGV (usar DIRECTORY_ROBOT_12)
#define ID_SEM_BLOQUEO_ROBOT_12         1

// ipcs entre robot 11 y 12 (usar DIRECTORY_ROBOT_12)
#define ID_COLA_11_A_12_1               1 // se lee cola de 11 a 12
#define ID_COLA_11_A_12_2               2
#define ID_COLA_12_A_11_1               3
#define ID_COLA_12_A_11_2               4

//mtype colas
#define TIPO_PEDIDO_ROBOT_5 			1 // Tipo utilizado entre la api del robot 5 y los controladores del mismo
#define TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5 	2 // Tipo utilizado entre la api del robot 5 y los controladores del mismo
#define TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5 3 // Tipo utilizado entre la api del robot 5 y los controladores del mismo

#define TIPO_PEDIDO_CANASTO 			1 // Tipo utilizado entre los AGV y el Robot 5
#define TIPO_PEDIDO_PRODUCCION 			1 // Tipo utilizado entre el almacen de piezas y Robot 5


//constantes del sistema
#define TAM_BUFFER 				255
#define BUFF_SIZE_CINTA_6               	5
#define CANTIDAD_CINTAS_6               	2
#define CANTIDAD_AGVS                           3
#define MAX_QUANTITY_CANASTOS                   50
#define MAX_PIEZAS_POR_PRODUCTO                 9
#define MAX_PIEZAS_CANASTO                      40

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
    int cantidadPiezas;
} Canasto;

typedef struct {
    TipoGabinete tipoGabinete;
} Gabinete;

/*
 * Estructuras utilizadas para la comunicacion entre el AGV y el robot 5 
 */

typedef struct {
    TipoPieza tipoPieza;
    int idAgv;
} PedidoCanastoAGV;

typedef struct {
	long mtype; // tipo = TIPO_PEDIDO_CANASTO
        PedidoCanastoAGV pedidoCanastoAgv;
} MensajePedidoAgv_5;

/*
 * Estrcuturas para la comunicacion entre el robot 5 y el robot 11.
 */

typedef struct {
    TipoProducto tipoProducto;
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
    bool robot5Bloqueado;
} EstadoRobot5;

/*
 * Estructuras utilizadas para la comunicacion entre el robot 5 y el almacen de piezas.
 */

typedef struct {
    int nroOrdenCompra;
    TipoProducto tipo;
    int cantidad;
    int diferenciaMinima;
} PedidoProduccion;

typedef struct {
    long mtype; // mtype = TIPO_PEDIDO_PRODUCCION
    PedidoProduccion pedidoProduccion;    
} MensajePedidoProduccion;

/*
 * Estructuras utilizadas internamente por el robot 5
 */

typedef struct {
    TipoPedidoRobot5 tipo;
    PedidoProduccion pedidoProduccion;
    PedidoCanastoAGV pedidoCanastoAgv;
} PedidoRobot5;

typedef struct {
    long mtype;
    PedidoRobot5 pedidoRobot5;
} MensajePedidoRobot5;

typedef struct {
    long mtype; // tipo = TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5
    int idAgv;
    Canasto canasto;
} MensajeRespuestaCanasto;

typedef struct {
    long mtype; // tipo = TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5
    ProductoEnProduccion productoEnProduccion;
    bool ultimo;
} MensajeRespuestaGabinete;

/*
 * Estructuras utilizadas entre AGVs, almacen de piezas, robot 11 y robot 12
 */

typedef struct {
    int robotCinta1EsperaPorElLugarNumero;
    int robotCinta2EsperaPorElLugarNumero;
    Canasto canastos[MAX_QUANTITY_CANASTOS];
} BufferCanastos;

/*
 * Estructuras utilizadas entre AGVs, robot 11 y robot 12
 */

typedef struct {
    TipoPieza tipoPieza;
    int lugar;
} PedidoCanastoRobotCitna6;

typedef struct {
	long mtype; // tipo = TIPO_PEDIDO_CANASTO
	PedidoCanastoRobotCitna6 pedidoCanastoAgv;
} MensajePedidoRobotCinta_6;

/*
 * Estructuras utilizadas entre robot 11 y robot 12
 */

typedef struct {
    long mtype; 
} MensajeBarrera;

typedef struct
{
    TipoPieza tipoPieza;
    int cantidad;
} EspecifPiezaDeProd;


typedef struct
{
    TipoProducto idProducto;
    int cantPiezas;
    EspecifPiezaDeProd tipoPantalla;
    EspecifPiezaDeProd pieza[MAX_PIEZAS_POR_PRODUCTO];    
} EspecifProd;

/*
 * Estructuras utilizadas entre robot 11, 14 y 16
 */

typedef struct {
    TipoProducto idProducto;
    int ordenDeCompra;
    bool fallado;
} Caja;

#endif	/* COMMON_H */