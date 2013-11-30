#ifndef MIDDLEWARECOMMON_H
#define	MIDDLEWARECOMMON_H

#define DIRECTORY_COMM  "/etc"
#define MSG_DEMUX       1




typedef enum {
    ID_TIPO_CLIENTE = 1,
    ID_TIPO_VENDEDOR,
    ID_TIPO_ROBOT5,
    ID_TIPO_AP,
    ID_TIPO_APT,
    ID_TIPO_ROBOT11,
    ID_TIPO_ROBOT12,
    ID_TIPO_AGV,
    ID_TIPO_ROBOT14,
    ID_TIPO_ROBOT16,
    ID_TIPO_DESPACHO
} TipoAgente;

#define DIR_FIXED_SIZE          30
#define DEMUX_FIXED_SIZE        100

typedef struct {
    int tamanioMensajeDemux;
    char mensajeDemux[DEMUX_FIXED_SIZE];
} msg_demux_t;

typedef struct {
    // Se utiliza como mtype el id del emisor, para que lo pueda recibir el 
    // canal indicado
    long mtype; 
    TipoAgente idTipoAgente;
    char directorioIPC[DIR_FIXED_SIZE];
    int idIPC;
    msg_demux_t mensaje;
} msg_a_canal_t;



#endif	/* MIDDLEWARECOMMON_H */

