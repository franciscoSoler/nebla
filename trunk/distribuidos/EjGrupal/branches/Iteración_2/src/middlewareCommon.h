#ifndef MIDDLEWARECOMMON_H
#define	MIDDLEWARECOMMON_H

#define DIRECTORY_COMM             "./DComm"
#define DIRECTORY_BROKER           "./DBroker"

#define SH_MEM_CONFIG_FILE          "SharedMemoryConfigFile.txt"

typedef enum {
    ID_TIPO_VACIO = 0,
    ID_TIPO_CLIENTE,
    ID_TIPO_VENDEDOR,
    ID_TIPO_ROBOT5_CINTA,
    ID_TIPO_ROBOT5_AGV,
    ID_TIPO_AP,
    ID_TIPO_ROBOT11,
    ID_TIPO_ROBOT12,
    ID_TIPO_AGV,
    ID_TIPO_ROBOT14,
    ID_TIPO_ROBOT16_CINTA,
    ID_TIPO_ROBOT16_DESPACHO,
    ID_TIPO_DESPACHO
} TipoAgente;

#define DIR_FIXED_SIZE          30
#define MSG_QUEUE_FIXED_SIZE    300
#define MSG_BROKER_SIZE         400

/* Nomenclatura: Todos los mensajes hacen referencia a su receptor. 
 * Ejemplo: MsgCanalSalidaBroker. Esto indica que es un mensaje que
 * va dirigido a un CanalSalida del Broker.
 */

typedef struct {
    long mtype;
    char msg[MSG_QUEUE_FIXED_SIZE];
} MsgAgenteReceptor;

typedef struct {
    char directorioIPC[DIR_FIXED_SIZE];
    int idIPC;
    MsgAgenteReceptor msg;
} MsgCanalEntradaAgente;

typedef struct {  
    long mtype; // idReceptor
    MsgCanalEntradaAgente msg;
} MsgCanalSalidaBroker;

typedef struct { 
    TipoAgente idTipoReceptor;
    long idReceptor;
    //MsgCanalSalidaBroker msg;
    char msg[MSG_BROKER_SIZE];
} MsgCanalEntradaBroker;

typedef struct {
    // Se utiliza como mtype el id del emisor, para que lo pueda recibir el 
    // canal indicado
    long mtype; 
    MsgCanalEntradaBroker msg;
} MsgCanalSalidaAgente;



#endif	/* MIDDLEWARECOMMON_H */

