#ifndef COMMON_H
#define	COMMON_H

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define NOMBRE_ARCHIVO_PRODUCTOS        "info_productos.csv"

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY_AGV                   "./DAGV"
#define DIRECTORY_ROBOT_5               "./DRobot5"
#define DIRECTORY_ROBOT_11              "./DRobot11"
#define DIRECTORY_ROBOT_12              "./DRobot12"
#define DIRECTORY_ROBOT_14              "./DRobot14"
#define DIRECTORY_ROBOT_16              "./DRobot16"
#define DIRECTORY_CLIENTE               "./DCliente"
#define DIRECTORY_VENDEDOR              "./DVendedor"
#define DIRECTORY_DESPACHO              "./DDespacho"
#define DIRECTORY_APT                   "./DAPT"
#define DIRECTORY_APIEZAS               "./DAPiezas"

//ipcs entre AGV y Robot5 (usar DIRECTORY_AGV)
#define ID_SEM_BLOQUEO_AGV              1   // ID para el semaforo de bloqueo de los AGV
#define ID_SEM_BUFFER_AGV_5            	2	// es el id para el semaforo de acceso al buffer
#define ID_BUFFER_AGV_5_0           	3	// es el id tanto para shMemory como semaforo de acceso
#define ID_BUFFER_AGV_5_1               4
#define ID_BUFFER_AGV_5_2               5
#define ID_COLA_PEDIDOS_AGV_5           6

// ipcs entre AGV, robot 11 y robot 12 (usar DIRECTORY_AGV)
#define ID_SEM_BUFFER_CANASTOS          7
#define ID_COLA_PEDIDOS_ROBOTS_AGV      8
#define ID_BUFFER_CANASTOS_0            9
#define ID_BUFFER_CANASTOS_1            10
#define ID_BUFFER_CANASTOS_2            11


//ipcs del Robot5 (usar DIRECTORY_ROBOT_5)
#define ID_SEM_API_ROBOT_5              1

//ipcs entre Robot5 y almacen de piezas (usar DIRECTORY_ROBOT_5)
#define ID_COLA_PEDIDOS_PRODUCCION      2

//ipcs entre robot 11 y robot 5 (usar DIRECTORY_ROBOT_5)
#define ID_SEM_BLOQUEO_ROBOT_5          3
#define ID_ESTADO_ROBOT_5               4 // Se utiliza el mismo para la sh mem y para el semaforo

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
#define ID_COLA_11_A_12               1 
#define ID_COLA_12_A_11               2

//ipcs del robot 14 (usar DIRECTORY_ROBOT_14)
#define SEM_R14_CINTA13_ID              1
#define SEM_R14_CINTA15_ID              4
#define SEM_MUTEX_SM_R14_R16_ID         2
#define SM_R14_R16_ID                   2

//ipcs del robot 16 (usar DIRECTORY_ROBOT_16)
#define SEM_MUTEX_SINCRONISMO_R16_ID     1
#define MSGQUEUE_R16_CINTA15_INPUT_ID    2
#define MSGQUEUE_R16_DESPACHO_INPUT_ID   3
#define MSGQUEUE_R16_CLIENT_ID           5

// ipcs del despacho (usar DIRECTORY_DESPACHO)
#define MSGQUEUE_DESPACHO_INPUT_ID      1

// ipcs del cliente (usar DIRECTORY_CLIENTE)
#define MSGQUEUE_CLIENT_INPUT_ID        1
#define MSGQUEUE_CLIENT_OUTPUT_ID       2

// ipcs del vendedor (usar DIRECTORY_VENDEDOR)
#define MSGQUEUE_VENDOR_INPUT_ID        1
#define MSGQUEUE_VENDOR_OUTPUT_ID       2

// ipcs del APT (usar DIRECTORY_APT)
#define SEM_MUTEX_SM_APT_ID             1


// ipcs del APiezas (usar DIRECTORY_APIEZAS)
#define LETRA_SHMEM_ALMACEN_PIEZAS      1
#define LETRA_SEM_ALMACEN_PIEZAS        1

#define LETRA_SEM_ESPERA_REPOSITOR_GABINETES 2
#define LETRA_SEM_ESPERA_REPOSITOR_CANASTOS  3

//mtype colas

#define TIPO_PEDIDO_CANASTO 			1 // Tipo utilizado entre los AGV y el Robot 5
#define TIPO_PEDIDO_PRODUCCION 			1 // Tipo utilizado entre el almacen de piezas y Robot 5
#define ID_ALMACEN_PIEZAS               1 // Tipo utilizado entre robot 5 almacen de productos

#define TIPO_PEDIDO_DESPACHO            1
#define TIPO_PEDIDO_ODC_DESPACHO        2


//constantes del sistema
#define TAM_BUFFER                              512
#define BUFF_SIZE_CINTA_6                       5
#define CANTIDAD_CINTAS_6                       2
#define CANTIDAD_AGVS                           3
#define MAX_QUANTITY_CANASTOS                   50
#define MAX_PIEZAS_POR_PRODUCTO                 9
#define MAX_PIEZAS_CANASTO                      40
#define AMOUNT_CINTA_13                         2
#define CINTA_13_CAPACITY                       3
#define CINTA_15_CAPACITY                       5
#define CANTIDAD_PRODUCTOS                      3
#define CANTIDAD_TIPOS_PIEZAS                   6
#define CANTIDAD_TIPOS_GABINETES                3
#define CANTIDAD_MAXIMA_REPOSICION_PIEZAS       200
#define CANTIDAD_MINIMA_REPOSICION_PIEZAS       100
#define CANTIDAD_MAXIMA_ITEMS_REPOSICION        10
#define CANTIDAD_MINIMA_ITEMS_REPOSICION    	5
#define CANTIDAD_MAXIMA_ITEMS_POR_TIPO_ALMACEN  15

/* Tipos de productos. */

#define CANT_MAX_PEDIDOS                        10
#define CANT_PRODUCTOS                          3

#define CANT_VENDEDORES                         10
#define CANT_CLIENTES                           50
#define TAM_ALMACEN                             30

#define CANT_MAX_COMPONENTES_PRODUCTO           10

/* Constantes IPC internas. */
// User DIRECTORY_VENDEDOR
#define ID_COLA_VENDEDORES                      'b'
#define ID_COLA_CLIENTES                        'i'
#define ID_COLA_PEDIDOS                         'j'

#define ID_COLA_VENDEDORES_C                    'y'
#define ID_COLA_CLIENTES_C                      'x'
#define ID_COLA_PEDIDOS_C                       'z'

#define ID_COLA_RESPUESTAS_ALMACEN_PIEZAS       'e'
#define ID_COLA_CONSULTAS_ALMACEN_PIEZAS        'f'
#define ID_ALMACEN_TERMINADOS                    1
#define ID_SHMEM_NRO_OC                          2

#define TIPO_BUSCANDO_VENDEDOR 1
#define TIPO_VENDEDOR_LIBRE 2

/* Constantes relacionadas con el Middleware */
#define SERVERS_CONFIG_FILE                      "ServersConfigFile.txt"

typedef enum {
    GABINETE_1 = 1,
    GABINETE_2 = 2,
    GABINETE_3 = 3
} TipoGabinete;

typedef enum {
    NULL_PIEZA = 0,
    PIEZA_1 = 1,
    PIEZA_2 = 2,
    PIEZA_3 = 3,
    PANTALLA_1 = 4,
    PANTALLA_2 = 5,
    PANTALLA_3 = 6
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
    // mtype = TIPO_PEDIDO_PRODUCCION
    PedidoProduccion pedidoProduccion;    
} MensajePedidoProduccion;

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
} PedidoCanastoRobotCinta6;

typedef struct {
	PedidoCanastoRobotCinta6 pedidoCanastoAgv;
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

// Clases y Estructuras generales
class Caja {
public:
    Caja() :  idOrdenDeCompra_(0)
            , idVendedor_(0)
            , idProducto_(NULL_PRODUCT)
            , fallado_(false) {}
    Caja(int ordenDeCompra) { idOrdenDeCompra_ = ordenDeCompra;};
    long getOrdenDeCompra() { return idOrdenDeCompra_;}
    bool estaVacio() { return idOrdenDeCompra_ == 0
            && idVendedor_ == 0 && idProducto_ == NULL_PRODUCT; };

public:
    long idOrdenDeCompra_;
    long idVendedor_;
    TipoProducto idProducto_;
    bool fallado_;
};


// FIXME: Refactorizar esto
class OrdenDeCompra {
public:
    OrdenDeCompra() :   idVendedor_(0),
                        idCliente_(0),
                        idOrden_(0),
                        faltantesAEntregar_(0) {

        for (int i = 0; i < CANTIDAD_PRODUCTOS; ++i) {
            productoTerminado_[i] = false;
            cantidadPorProducto_[i] = false;
        }

    }

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
    int faltantesAEntregar_;
    int cantidadPorProducto_[CANTIDAD_PRODUCTOS];
    // Indica con true si la cantidad pedida ya se encontraba stockeada.
    bool productoTerminado_[CANTIDAD_PRODUCTOS];

};

class PedidoOrdenDeCompra {
public:
    PedidoOrdenDeCompra() {
        mtype = TIPO_PEDIDO_DESPACHO;   
    }
public:
    long mtype;
    OrdenDeCompra ordenDeCompra_;
};

typedef enum {
    PEDIDO_VACIO = 0,
    PEDIDO_ODC = 1,
    PEDIDO_CLIENTE,
    PEDIDO_ROBOT16
} TipoPedidoDespacho;


class PedidoDespacho {
public:
    PedidoDespacho() :  tipoPedido_(PEDIDO_VACIO),
                        idProducto_(NULL_PRODUCT),
                        idCliente_(0),
                        idOrdenDeCompra_(0)
    {}

public:
    TipoPedidoDespacho tipoPedido_;
    TipoProducto idProducto_;
    long idCliente_;
    long idOrdenDeCompra_;
    OrdenDeCompra odc_;
};


/*** Mensajes de colas, con sus respectivos IDs de mensajes definidos ***/

// Utilizado por las colas que intercambian info con el despacho
#define ID_DESPACHO             1
class Msg_PedidoDespacho {
public:
    Msg_PedidoDespacho() {}

public:
    PedidoDespacho pedido_;
    bool ultimoPedido_;
};

// Mensaje del Robot14 al Robot16
#define MSG_AVISO_CAJA_CINTA_15         1
class Msg_AvisoCajaEnCinta15 {
public:
    Msg_AvisoCajaEnCinta15() {}
public:
    // FIXXXXME: Please, do not touch this struct. 
    // The universe could collapse
    long prrrrrrr;
};

#define ID_R16_DESPACHO            1
class Msg_FinProductoR16 {
public:
    Msg_FinProductoR16() {}
public:
    PedidoDespacho pedido_;
    bool ultimoProductoDeODC_;
};

#define MSG_RETIRO_PRODUCTO             5
class Msg_RetiroProducto {
public:
    Msg_RetiroProducto() {}
public:
    PedidoDespacho datos_;
    bool ultimoPedido_;
};

class Msg_EnvioCajaCliente {
public:
    Caja caja;
    bool ultimoProductoDeODC_;
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
    long idOrdenDeCompra;
    int tipoProducto;
    Caja caja;
    bool esTemporal;
} EspacioAlmacenProductos;


typedef struct 
{
    EspacioAlmacenProductos almacen[TAM_ALMACEN];
} AlmacenProductosTerminados;


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
    pedido_fabricacion_t pedidoFabricacion; 
} mensaje_pedido_fabricacion_t;

/* 
 * Etructuras utilizadas entre el cliente y el vendedor
 */

typedef struct _mensaje_inicial
{
    // mtype = receptor.
    long emisor; 
} mensaje_inicial_t;

typedef struct _pedido
{
    long emisor;
    TipoProducto tipoProducto;
    int cantidad;
    bool fin;
} pedido_t;

typedef struct {
    // receptor.
    int tipo; // indica si se debe terminar la comunicacion
    pedido_t pedido;
} msg_pedido_t;

typedef struct _respuesta_pedido {
    long emisor;
    bool recepcionOK;
    int cantidadDeProductos[CANTIDAD_PRODUCTOS];
} respuesta_pedido_t;

typedef struct {
    int tipo;
    respuesta_pedido_t respuesta_pedido;
} msg_respuesta_pedido_t;

typedef struct _espacio_almacen_piezas {
    Canasto canastos[CANTIDAD_TIPOS_PIEZAS][CANTIDAD_MAXIMA_ITEMS_POR_TIPO_ALMACEN];
    Gabinete gabinetes[CANTIDAD_TIPOS_GABINETES][CANTIDAD_MAXIMA_ITEMS_POR_TIPO_ALMACEN];
    int cantCanastos[CANTIDAD_TIPOS_PIEZAS];
    int cantGabinetes[CANTIDAD_TIPOS_GABINETES];
} EstructuraAlmacenPiezas;

/* Comunicacion de red entre clientes y vendedores */

typedef struct {
    int numero;
} StartComunicationMessage;

typedef struct {
    int origen;  // Unused
    int destino; // Unused
    int tipo;    //Utilizado para indicar cuando finalizar la comunicacion
    size_t size; // Unused
    msg_respuesta_pedido_t mensaje;
} net_msg_respuesta_pedido_t;

typedef struct {
    int origen;  // Unused
    int destino; // Unused
    int tipo;    //Utilizado para indicar cuando finalizar la comunicacion
    size_t size; // Unused
    msg_pedido_t mensaje;
} net_msg_pedido_t;

#endif	/* COMMON_H */
