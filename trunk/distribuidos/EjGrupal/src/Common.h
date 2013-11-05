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
#define DIRECTORY_ROBOT_14 	"./DRobot14"
#define DIRECTORY_ROBOT_16 	"./DRobot16"
#define DIRECTORY_CLIENTE       "./DCliente"
#define DIRECTORY_VENDEDOR 	"./DVendedor"
#define DIRECTORY_DESPACHO      "./DDespacho"
#define DIRECTORY_APT           "./DAPT"

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

// ipcs entre robot 11 y robot 14 (usar DIRECTORY_ROBOT_11)
#define SEM_R11_CINTA_13                5
#define SEM_MUTEX_SM_R11_R14_ID         6
#define SM_R11_R14_ID                   6

// ipcs entre robot 12 y AGV (usar DIRECTORY_ROBOT_12)
#define ID_SEM_BLOQUEO_ROBOT_12         1

// ipcs entre robot 11 y 12 (usar DIRECTORY_ROBOT_12)
#define ID_COLA_11_A_12_1               1 // se lee cola de 11 a 12
#define ID_COLA_11_A_12_2               2
#define ID_COLA_12_A_11_1               3
#define ID_COLA_12_A_11_2               4

//ipcs del robot 14 (usar DIRECTORY_ROBOT_14)
#define SEM_R14_ID                      1
#define SEM_MUTEX_SM_R14_R16_ID         2
#define SM_R14_R16_ID                   2

//ipcs del robot 16 (usar DIRECTORY_ROBOT_16)
#define SEM_R16_ID                      1
#define MSGQUEUE_ROBOT16_INPUT_ID       1
#define MSGQUEUE_ROBOT16_OUTPUT_ID      2
#define MSGQUEUE_R16_CLIENT_ID          3

// ipcs del despacho (usar DIRECTORY_DESPACHO)
#define MSGQUEUE_DESPACHO_OUTPUT_ID     1

// ipcs del cliente (usar DIRECTORY_CLIENTE)
#define MSGQUEUE_CLIENT_INPUT_ID        1
#define MSGQUEUE_CLIENT_OUTPUT_ID       2

// ipcs del vendedor (usar DIRECTORY_VENDEDOR)
#define MSGQUEUE_VENDOR_INPUT_ID        1
#define MSGQUEUE_VENDOR_OUTPUT_ID       2

// ipcs del APT (usar DIRECTORY_APT)
#define LETRA_SHMEM_ALMACEN_TERMINADOS  'a'
#define SEM_MUTEX_SM_APT_ID             1



//mtype colas
#define TIPO_PEDIDO_ROBOT_5 			1 // Tipo utilizado entre la api del robot 5 y los controladores del mismo
#define TIPO_MENSAJE_RESPUESTA_CANASTO_ROBOT_5 	2 // Tipo utilizado entre la api del robot 5 y los controladores del mismo
#define TIPO_MENSAJE_RESPUESTA_GABINETE_ROBOT_5 3 // Tipo utilizado entre la api del robot 5 y los controladores del mismo


#define TIPO_PEDIDO_CANASTO 			1 // Tipo utilizado entre los AGV y el Robot 5
#define TIPO_PEDIDO_PRODUCCION 			1 // Tipo utilizado entre el almacen de piezas y Robot 5
#define TIPO_PEDIDO_ROBOT_5_ALMACEN_PIEZAS	2 // Tipo utilizado entre robot 5 almacen de productos

#define TIPO_PEDIDO_DESPACHO            1
#define TIPO_PEDIDO_ODC_DESPACHO        2


//constantes del sistema
#define TAM_BUFFER 				255
#define BUFF_SIZE_CINTA_6               	5
#define CANTIDAD_CINTAS_6               	2
#define CANTIDAD_AGVS                           3
#define MAX_QUANTITY_CANASTOS                   50
#define MAX_PIEZAS_POR_PRODUCTO                 9
#define MAX_PIEZAS_CANASTO                      40
#define AMOUNT_CINTA_13                         2
#define CINTA_13_CAPACITY                       3
#define CINTA_15_CAPACITY                       5
#define CANTIDAD_PRODUCTOS                      3


/* Tipos de productos. */

#define CANT_MAX_PEDIDOS 10
#define CANT_PRODUCTOS 3

#define CANT_VENDEDORES 10
#define CANT_CLIENTES 50
#define TAM_ALMACEN 30

#define CANT_MAX_COMPONENTES_PRODUCTO 10

// comunicación robot 5

/* Constantes IPC internas. */
// User DIRECTORY_VENDEDOR
#define ID_COLA_VENDEDORES 'b'
#define ID_COLA_CLIENTES 'i'
#define ID_COLA_PEDIDOS 'j'
#define ID_COLA_RESPUESTAS_ALMACEN_PIEZAS 'e'
#define ID_COLA_CONSULTAS_ALMACEN_PIEZAS 'f'
#define ID_ALMACEN_TERMINADOS 'c'
#define ID_SHMEM_NRO_OC 'g'

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
    PIEZA_1 = 1,
    PIEZA_2 = 2,
    PIEZA_3 = 3,
    PANTALLA_1 = 4,
    PANTALLA_2 = 5,
    PANTALLA_3
} TipoPieza;

typedef enum {
    NULL_PRODUCT = 0,
    PRODUCTO_1 = 1,
    PRODUCTO_2 = 2,
    PRODUCTO_3
} TipoProducto;

typedef enum  { 
    CANT_MINIMA_FABRICACION, 
    PRODUCTOS_NECESARIOS, 
    ORDEN_PRODUCCION 
} TipoConsulta;

typedef enum { 
    VACIO, 
    PRODUCTO_PARA_ENTREGAR, 
    RESERVADO_DISPONIBLE, 
    RESERVADO_VENDIDO, 
    PRODUCTO_DISPONIBLE 
} EstadoEspacio;

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

typedef struct {
    long mtype; 
} MensajeProximoPedidoProduccion;

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
class Caja {
public:
    Caja() {ordenDeCompra_ = 0;};
    Caja(long ordenDeCompra) { ordenDeCompra_ = ordenDeCompra;};
    long getOrdenDeCompra() { return ordenDeCompra_;};
    bool estaVacio() { return ordenDeCompra_ == 0; };
    
public:
    long ordenDeCompra_;
    TipoProducto idProducto_;
    bool fallado_;
};


// FIXME: Refactorizar esto
class OrdenDeCompra {
public:
    OrdenDeCompra() :   idVendedor_(0),
                        idCliente_(0),
                        idOrden_(0) {}
    
    virtual ~OrdenDeCompra() {}
  
    void decrementarProducto(TipoProducto producto) {
        if (cantidadPorProducto_[producto] <= 0 || producto > CANTIDAD_PRODUCTOS) {
            //throw Exception("OrdenDeCompra Error: Tipo de producto inválido");
        }

        if (cantidadPorProducto_[producto-1] <= 0) {
            //throw Exception("OrdenDeCompra Error: Producto agotado");
        }

        cantidadPorProducto_[producto-1]--;
    }

    bool estaVacia() const {
        return idVendedor_ == 0 && idCliente_== 0 && idOrden_ == 0; 
    }

public:
    long idVendedor_;
    long idCliente_;
    long idOrden_;
    int cantidadPorProducto_[CANTIDAD_PRODUCTOS];

};

typedef enum {
    PEDIDO_VACIO = 0,
    PEDIDO_ODC = 1,
    PEDIDO_CLIENTE,
    PEDIDO_ROBOT16
} TipoPedidoDespacho;

// Utilizado por las colas que intercambian info con el despacho
class PedidoDespacho {
public:
    PedidoDespacho() :  mtype(TIPO_PEDIDO_DESPACHO),
                        tipoPedido_(PEDIDO_VACIO),
                        idProducto_(NULL_PRODUCT),
                        idCliente_(0),
                        idOrdenDeCompra_(0)
    {}
    
public:
    long mtype;
    TipoPedidoDespacho tipoPedido_;
    TipoProducto idProducto_;
    long idCliente_;
    long idOrdenDeCompra_;
};

typedef struct {
    long mtype;
    Caja caja;
} EnvioCajaCliente;

class PedidoOrdenDeCompra {
public:
    PedidoOrdenDeCompra() {
        mtype = TIPO_PEDIDO_DESPACHO;   
    }
public:
    long mtype;
    OrdenDeCompra ordenDeCompra_;
};


/* 
 * Cliente - Vendedor - Almacen de piezas - Almacen productos terminados 
 */

typedef struct _OrdenProduccion
{
    int vendedor;
    int tipoProducto;
} OrdenDeProduccion;



typedef struct _EspacioAlmacen
{
    EstadoEspacio estado;
    OrdenDeCompra ordenCompra;
    int tipoProducto;
    Caja caja;
    bool esTemporal;
} EspacioAlmacenProductos;



typedef struct _consulta_almacen_piezas
{
    long mtype; // receptor.
    long emisor;
    TipoProducto tipoProducto;
    int cantidad;
    int cliente;
    int numOrdenCompra;
    int diferencia;
    TipoConsulta tipoConsulta; 
} consulta_almacen_piezas_t;

typedef struct _respuesta_almacen_piezas
{
    long mtype; // receptor.
    long emisor;
    int tipoProducto;
    int tipo;
    int cantidad;
} respuesta_almacen_piezas_t;

/* 
 * Estructura utilizadas para la comunicacion entre vendedor y almacen de piezas 
 */

typedef struct _pedido_produccion
{
    bool ventaEsValida;
    int producidoParaStockear;
    int producidoVendido;
    int vendidoStockeado;
    int diferenciaMinimaProducto;
    int numOrdenCompra;
    TipoProducto tipoProducto;
} pedido_fabricacion_t;

typedef struct {
    long mtype;
    pedido_fabricacion_t pedidoFabricacion; 
} mensaje_pedido_fabricacion_t;

/* 
 * Etructuras utilizadas entre el cliente y el vendedor
 */

typedef struct _mensaje_inicial
{
    long mtype; // receptor.
    long emisor; 
} mensaje_inicial_t;

typedef struct _pedido
{
    long emisor;
    int numMensaje;
    TipoProducto tipoProducto;
    int cantidad;
    bool fin;
} pedido_t;

typedef struct {
    long mtype; // receptor.
    pedido_t pedido;
} msg_pedido_t;

typedef struct _respuesta_pedido
{
    long emisor;
    bool recepcionOK;
} respuesta_pedido_t;

typedef struct {
    long mtype;
    respuesta_pedido_t respuesta_pedido;
} msg_respuesta_pedido_t;

#endif	/* COMMON_H */