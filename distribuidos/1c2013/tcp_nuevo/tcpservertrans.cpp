/*
 * Procedimiento tcptranv()
 * Descripcion:	Transferencia de datos entre server y cliente
 *               Recibe una linea del cliente
 *		Le antepone la hora de recepcion
 *		Devuelve el mensaje al cliente
 *  		Retorna.
 * Sintaxis: tcpservertrans(sockfd, server, pid_eco)
 *	sockfd:	socket del accept
 *	server:	nombre del programa server
 *      pid_eco:    pid del eco que atiende al cliente
 * Usa:		funcion recibir()
 *		         funcion enviar()
 */

#include	"inet.h"
#include   <time.h>
#define	MAXLINEA 1024
extern int recibir(int, void *, size_t );
extern int enviar(int, void *, size_t);

void tcpservertrans(int sockfd, char server[], pid_t pid_eco)
{
	int	n;	
	char	mostrar[80];   /* mensaje para mostrar en pantalla */
	char	linea[MAXLINEA];
	char    salida[MAXLINEA + 28];
	char 	txt_err[] = "Error en el read";
	time_t  ticks;
	for ( ; ; ) 
		{
		n = recibir(sockfd, linea, MAXLINEA);
		if (n == 0)
		    return;		/* fin de la conexion */
		else
		    if (n < 0)
			{
		      sprintf(mostrar,"%s (%d): error en recibir\n", server, pid_eco);
                        write(fileno(stdout), mostrar, strlen(mostrar));
			n = strlen(txt_err);	/* devuelve error */	
			if (enviar(sockfd, (char *)txt_err, n) != n) {
				sprintf(mostrar,"%s (%d): error en enviar\n", server, pid_eco);
                                write(fileno(stdout), mostrar, strlen(mostrar));
                              }
			}
	         ticks = time(NULL);
                  sprintf(mostrar,"\n%s (%d):  mensaje recibido = ", server, pid_eco);
                  write(fileno(stdout), mostrar, strlen(mostrar));
		/* 
		*	  anteponer la hora de recepcion, mostrar y devolver el mensaje recibido
		 */
                  sprintf(salida, "%.24s -> %s", ctime(&ticks), linea);
	         write(fileno(stdout), salida, strlen(salida));
		if (enviar(sockfd, (char *) salida, MAXLINEA + 28) != MAXLINEA + 28) {
			sprintf(mostrar,"%s (%d): error en enviar\n", server, pid_eco);
                           write(fileno(stdout), mostrar, strlen(mostrar));
                    }
	}
}
