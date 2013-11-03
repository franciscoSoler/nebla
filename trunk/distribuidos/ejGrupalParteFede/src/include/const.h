/* 
 * File:   const.h
 * Author: fede
 *
 * Created on October 27, 2013, 11:41 AM
 */

#ifndef CONST_H
#define	CONST_H

/* Constantes IPC internas. */
#define NOMBRE_ARCHIVO "/home/"
#define LETRA_COLA_VENDEDORES 'b'
#define LETRA_COLA_CLIENTES 'z'
#define LETRA_COLA_RESPUESTAS_ALMACEN_PIEZAS 'e'
#define LETRA_COLA_CONSULTAS_ALMACEN_PIEZAS 'f'
#define LETRA_SHMEM_ALMACEN_TERMINADOS 'c'
#define LETRA_SHMEM_NRO_OC 'g'
#define LETRA_SHMEM_NRO_OP 'h'
#define LETRA_SEM_ALMACEN_TERMINADOS 'd'

// comunicación robot 5
#define NOMBRE_ARCHIVO_ROBOT5 "/home/"
#define LETRA_ROBOT5 '2'
#define TIPO_ROBOT_5 1

/* Tipos de productos. */
#define NOMBRE_ARCHIVO_PRODUCTOS "../info_productos.csv"

#define CANT_MAX_PEDIDOS 10
#define CANT_PRODUCTOS 3
#define PRODUCTO_1 0
#define PRODUCTO_2 1
#define PRODUCTO_3 2

#define FIN_ENVIO -1
#define CANT_VENDEDORES 10
#define CANT_CLIENTES 50
#define TAM_ALMACEN 30

#define CANT_MAX_COMPONENTES_PRODUCTO 10

/* Tipos de mensajes y datos. */

typedef struct _pedido_produccion
{
    bool ventaEnCurso;
    int producidoParaStockear;
    int producidoVendido;
    int vendidoStockeado;
    int diferenciaMinimaProducto;
    int numOrdenCompra;
    int tipoProducto;
} pedido_produccion_t;

typedef struct _mensaje_inicial
{
    long mtype; // receptor.
    long emisor; 
} mensaje_inicial_t;

typedef struct _pedido
{
    long mtype; // receptor.
    long emisor;
    int numMensaje;
    int tipoProducto;
    int cantidad;
} pedido_t;

enum EstadoEspacio { VACIO, PRODUCTO_PARA_ENTREGAR, RESERVADO_DISPONIBLE, RESERVADO_VENDIDO, PRODUCTO_DISPONIBLE };

typedef struct _OrdenCompra
{
    int vendedor;
    int cliente;
    int numero;
} OrdenDeCompra;

typedef struct _OrdenProduccion
{
    int vendedor;
    int numero;
    int tipoProducto;
} OrdenDeProduccion;

typedef struct _Caja
{
    int tipoProducto;
    long vendedor;
    int idOrdenCompra;
} Caja;

typedef struct _EspacioAlmacen
{
    EstadoEspacio estado;
    OrdenDeCompra ordenCompra;
    OrdenDeProduccion ordenProduccion;
    Caja caja;
} EspacioAlmacenProductos;

enum TipoConsulta { CANT_MINIMA_FABRICACION, PRODUCTOS_NECESARIOS, ORDEN_PRODUCCION };

typedef struct _consulta_almacen_piezas
{
    long mtype; // receptor.
    long emisor;
    int tipoProducto;
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

/* Comunicación con robot 5. */
typedef enum {
    _PRODUCTO_1 = 0,
    _PRODUCTO_2 = 1,
    _PRODUCTO_3
} _TipoProducto;

typedef struct {
    int nroOrdenCompra;
    _TipoProducto tipo;
    int cantidad;
    int diferenciaMinima;
} PedidoProduccion;

typedef struct {
    long mtype; // mtype = TIPO_PEDIDO_PRODUCCION
    PedidoProduccion pedidoProduccion;    
} MensajePedidoProduccion;


#endif	/* CONST_H */

