/* 
 * File:   Common.h
 * Author: francisco
 *
 * Created on 24 de octubre de 2013, 21:07
 */

#ifndef COMMON_H
#define	COMMON_H


#include <string.h>
#include <stdio.h>
#include <errno.h>

// Path utilizado para inicializar los semáforos a través de la función ftok()
#define DIRECTORY "."

// Ids de los IPCs
#define ID_SEM_ROBOT_11                 1
#define ID_SEM_ROBOT_12                 2
#define ID_SEM_MEM_CINTA_6              3
#define ID_SEM_MEM_CANASTOS_10          4
#define ID_SEM_EST_AGV                  5
#define ID_SEM_EST_ROBOT_5              6
#define ID_SEM_MEM_EST_ROBOT_5          7
#define ID_SEM_MEM_AGV_5                8
#define ID_COLA_PEDIDOS_A_AGV           9
#define ID_COLA_PEDIDOS_AGV_5           10
#define ID_COLA_11_12_1                 11 // se lee cola de 11 a 12
#define ID_COLA_11_12_2                 12
#define ID_COLA_12_11_1                 13
#define ID_COLA_12_11_2                 14
#define ID_MEM_BUFFER_CANASTOS_1        15
#define ID_MEM_BUFFER_CANASTOS_2        16
#define ID_MEM_BUFFER_CANASTOS_3        17
#define ID_MEM_BUFFER_CANASTO_AGV_5_1   18
#define ID_MEM_BUFFER_CANASTO_AGV_5_2   19
#define ID_MEM_BUFFER_CANASTO_AGV_5_3   20
#define ID_MEM_CINTA_6_1                21
#define ID_MEM_CINTA_6_2                22
#define ID_MEM_ESTADO_ROBOT_5           23

//constantes del sistema
#define MAX_PIEZAS_POR_PRODUCTO         10
#define BUFF_SIZE_CINTA_6               5
#define BUFF_SIZE_CINTA_13              3
#define MAX_QUANTITY_CANASTOS           50
#define CANTIDAD_AGVS                   3
#define CANTIDAD_CINTAS_6               2

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
    ID_PROD1 = 0,
    ID_PROD2 = 1,
    ID_PROD3
} IdProd;

typedef struct {
    TipoGabinete gabinete;  
    int nroOrdenCompra;
} ProductoEnProduccion;

/* Cinta transportadora */

typedef struct
{
    TipoPieza tipoPieza;
    int cantidad;
} EspecifPiezaDeProd;


typedef struct
{
    IdProd idProducto;
    int numOrdenCompra;
    int cantPiezas;
    EspecifPiezaDeProd tipoPantalla;
    EspecifPiezaDeProd pieza[MAX_PIEZAS_POR_PRODUCTO];    
} EspecifProd;

typedef struct {
    bool robot11Durmiendo;
    int puntoLectura;
    int puntoEscritura;
    int cantLibres;
    bool lugarVacio[BUFF_SIZE_CINTA_6];
    ProductoEnProduccion gabinetes[BUFF_SIZE_CINTA_6];
    EspecifProd especificacionesProd[BUFF_SIZE_CINTA_6];
} CintaTransportadora_6;

// La especificación del tipo de pieza y la cantidad de la misma
// que tiene un producto 

typedef struct
{
    long mtype;
    //bool pedidoEsDeDeposito;
    TipoPieza tipoPieza;
    int lugar;
} PedidoCanasto;

typedef struct {
    TipoPieza tipoPieza;
    int cantidadPiezas;
} Canasto;

typedef struct {
    int robotCinta1EsperaPorElLugarNumero;
    int robotCinta2EsperaPorElLugarNumero;
    Canasto canastos[MAX_QUANTITY_CANASTOS];
} BufferCanastos;

typedef int IDAgv;

typedef struct {
    bool robot5Durmiendo;
} EstadoRobot5;

typedef struct {
    long mtype;
    TipoPieza tipo;
    IDAgv idAgv;
} PedidoCanastoAGV;

typedef struct {
    long mtype; 
} MessageBarrera;

typedef struct {
    IdProd idProducto;
    int ordenDeCompra;
} Caja;

typedef struct {
    //TODO arreglar esto, no creo que lo necesite del todo dado que eso lo hizo ezequiel!!!!!!
    int puntoLectura;
    int puntoEscritura;
    int cantLibres;
    bool lugarVacio[BUFF_SIZE_CINTA_13];
    Caja gabinetes[BUFF_SIZE_CINTA_13];
} CintaTransportadora_13;

#endif	/* COMMON_H */

