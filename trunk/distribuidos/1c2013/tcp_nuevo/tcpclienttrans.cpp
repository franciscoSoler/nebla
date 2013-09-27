/*
 * Procedimiento tcpclienttrans():
 * Descripcion: transferencia de datos entre cliente y servidor:
 *	Repetir hasta que el usuario ingrese un mensaje vacio ([enter] solamente) 
 *		el usuario ingresa  el mensaje  por teclado)
  *		Escribirlo sobre el socket, para enviarlo al server 
 *		Leer la respuesta del server que contiene la hora de recepcion
 *		Mostrar la respuesta en el stdout (la pantalla).
 *	Retorno.
 * Sintaxis: tcpclienttrans(int socketfd, char *cliente, pid_t pid_cl)
 *	socketfd = puntero al socket conectado con el server
 *	cliente = nombre del programa cliente
 *      pid_cl = pid del cliente
 * Usa:		funcion enviar()
 *		         funcion recibir()
 */

#include	"inet.h"
#define	MAXLINEA 1024
extern int recibir(int, void *, size_t );
extern int enviar(int, void *, size_t);

void tcpclienttrans (int sockfd, char *cliente, pid_t pid_cl)
{
 size_t	n;
 int	i;
 
 char 	mostrar[120];                        /* mensaje para la pantalla */
 char	sendlinea[MAXLINEA], recvlinea[MAXLINEA+28];
/* 
	 Repetir hasta que se ingrese un EOF */
 
 sprintf(mostrar,"%s (%d): Ingrese el mensaje para el servidor de eco o [enter] para terminar\n", cliente, pid_cl); /* pedir datos en pantalla   */
 write(fileno(stdout), mostrar, strlen(mostrar));
 i=0;
sendlinea[i] = getchar();
 while (sendlinea[0] != '\n') {
    while (i < MAXLINEA &&  sendlinea[i] != '\n') 
	{ i++;
	  sendlinea[i] = getchar();
	}
	 i++;
	 for (; i < MAXLINEA; i++) sendlinea[i] = '.';
 /*
  *	armar la linea a enviar al server
  *	enviar al server
  */
	if (enviar(sockfd, sendlinea, MAXLINEA) != MAXLINEA)   
	   {
	    sprintf(mostrar,"%s (%d): error en envio sobre el socket\n", cliente, pid_cl);
	    write(fileno(stdout), mostrar, strlen(mostrar));
            exit (1);
	   }

 /*
  *	recibir del server
  *	mostrar el mensaje en pantalla (stdout)
  */
 	n = recibir(sockfd, recvlinea, MAXLINEA+28);
 	if (n < 0)
	        {
		sprintf(mostrar,"%s (%d): error en recibir\n", cliente, pid_cl);
		write(fileno(stdout), mostrar, strlen(mostrar));
		}
 	else      
		{
		recvlinea[n] = 0;	/* pone \0 para terminar el string */
		sprintf(mostrar,"%s (%d): eco recibido = ", cliente, pid_cl);
		write(fileno(stdout), mostrar, strlen(mostrar));
		write(fileno(stdout), recvlinea, strlen(recvlinea));
		}

  	sprintf(mostrar,"\n %s (%d): Ingrese el mensaje para el servidor de eco o [enter] para terminar\n", cliente, pid_cl); /* pedir datos en pantalla   */
	write(fileno(stdout), mostrar, strlen(mostrar));
        i=0;
        sendlinea[i] = getchar();
	}
}
