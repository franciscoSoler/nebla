#ifndef MIDDLEWARECOMMON_H
#define	MIDDLEWARECOMMON_H

#define DIRECTORY_COMM             "./DComm"
#define DIRECTORY_BROKER           "./DBroker"
#define DIRECTORY_ADM              "./DAdm"
#define DIRECTORY_INFO_AGENTES     "./DInfoAgentes"
#define DIRECTORY_SEM              "./DSem"

#define COMM_OBJECTS_CONFIG_FILE   "CommObjectsConfigFile.txt"

/* Esto sólo lo vamos a utilizar para poder correr el sistema en una PC
 */

#define DIRECTORY_BROKER_1         "./DBroker1"
#define DIRECTORY_BROKER_2         "./DBroker2"
#define DIRECTORY_BROKER_3         "./DBroker3"
#define DIRECTORY_BROKER_4         "./DBroker4"
#define DIRECTORY_ADM_1            "./DAdm1"
#define DIRECTORY_ADM_2            "./DAdm2"
#define DIRECTORY_ADM_3            "./DAdm3"
#define DIRECTORY_ADM_4            "./DAdm4"
#define DIRECTORY_INFO_AGENTES_1   "./DInfoAgentes1"
#define DIRECTORY_INFO_AGENTES_2   "./DInfoAgentes2"
#define DIRECTORY_INFO_AGENTES_3   "./DInfoAgentes3"
#define DIRECTORY_INFO_AGENTES_4   "./DInfoAgentes4"

typedef enum {
    AGENTE = 1,
    ADMINISTRADOR_MEMORIA,
    BROKER
} TipoReceptor;

typedef enum {
    ID_TIPO_MEMORIA = 13,
    ID_TIPO_PEDIDO_MEMORIA
} CommTipoMensajeAgentes;

typedef enum {
    AGENTE_AGENTE = 15,
    MEMORIA_AGENTE,
    MENSAJE_LIDER,
    MEMORIA_BROKERS,
    AGENTE_CONECTADO,
    MENSAJE_BROADCAST
} CommTipoMensajeBrokers;


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
#define DIRECC_FIXED_SIZE       30
#define MSG_QUEUE_FIXED_SIZE    3000
#define MSG_BROKER_SIZE         3500
// Ver bien el tema de nombres de estos dos últimos defines
#define MSG_BROKER_FIXED_SIZE   3750
#define SEM_ARRAY_MAX_SIZE      10

// Cantidad de agentes por tipo. Se utiliza en la shMem que posee
// la información de que agente está conectado en cada Broker
#define MAX_AMOUNT_AGENTS       10
// Cantidad de Agentes en el sistema. Equivale al tamaño del
// struct TipoAgente - 1 (por el tipo ID_TIPO_VACIO)
#define AMOUNT_AGENTS           12

#define ID_SHMEM_SIGUIENTE      1
#define ID_COMM_SEM_ENTRADA     2

#define ID_IPC_INFO_GRUPOS_BROKERS  6

// CSBB: CanalSalidaBrokerBroker
#define ID_MSG_QUEUE_CSBB       20
#define ID_INFO_AGENTES         4

#define ID_ALGORITMO_LIDER      21

#define ID_SHMEM_TIMEOUT        3
#define ID_SEM_TIMEOUT          3

/* Define la cantidad de tiempo en segundos que se va a esperar
 * antes de que se haga timeout. */
#define TIEMPO_TIMEOUT          10

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
    TipoAgente idReceiverAgentType;
    long idReceptor;
} DireccionamientoMsgAgente;

typedef struct {
    CommTipoMensajeAgentes idMsgAdmType;
    long idMemory;
} DireccionamientoMsgAdministrador;

typedef struct {
    CommTipoMensajeBrokers idMsgBrokerType;
} DireccionamientoMsgBroker;

typedef struct {  
    long mtype; // idReceptor
    TipoAgente idTipoEmisor;
    long idEmisor;
} MsgPedidoMemoriaAdministrador;

typedef struct {  
    long mtype; // idReceptor
    char memoria[MSG_QUEUE_FIXED_SIZE];
} MsgEntregaMemoriaAdministrador;


typedef struct { 
    TipoReceptor receiverType;
    char direccionamiento[DIRECC_FIXED_SIZE];
    // MsgCanalSalidaBroker o MsgPedidoMemoriaAdministrador o MsgEntregaMemoriaAdministrador
    char msg[MSG_BROKER_SIZE];
} MsgCanalEntradaBroker;

typedef struct {
    // Se utiliza como mtype el id del emisor, para que lo pueda recibir el 
    // canal indicado
    long mtype; 
    MsgCanalEntradaBroker msg;
} MsgCanalSalidaAgente;

typedef enum {
    INICIAR,
    DESCONOCIDO,
    LIDER
} StatusLider;

typedef struct {
    long mtype; //Id del grupo
    int uid;
    StatusLider status;
} MsgAlgoritmoLider;

/* Estructuras utilizadas comunicación entre Brokers
 */

typedef struct {
    CommTipoMensajeBrokers tipoMensaje;
    char msg[MSG_BROKER_FIXED_SIZE];
} MsgCanalEntradaBrokerBroker;

typedef struct {
    long mtype;
    MsgCanalEntradaBrokerBroker msg;
} MsgCanalSalidaBrokerBroker;

typedef struct {
    // Debería ser un TipoAgente en vez de un int
    int idTipoAgente;
    long idAgente;
    int idBroker;
} TriadaInfoAgente;

#define CANT_GRUPOS_SHMEM       14
#define CANT_BROKERS            4
#define NOMBRE_ARCHIVO_GRUPOS   "infoShMemTipoAgente.csv"
#define ID_PRIMER_GRUPO_SHMEM   400

typedef struct _InformacionGrupoMemBrokers
{
    unsigned int tiposDeAgenteNecesariosPorGrupo[CANT_GRUPOS_SHMEM][AMOUNT_AGENTS];
    int tiposDeAgenteRestantePorGrupo[CANT_GRUPOS_SHMEM][AMOUNT_AGENTS];
    bool grupoCompleto[CANT_GRUPOS_SHMEM];
} InformacionGrupoShMemBrokers;



class DataInfoAgentes {
public:
    DataInfoAgentes() {
        for (int i = 0; i < MAX_AMOUNT_AGENTS; ++i) {
            agenteEnBroker[i] = 0;
        }
    }

public:
    int agenteEnBroker[MAX_AMOUNT_AGENTS];
};

#endif	/* MIDDLEWARECOMMON_H */

