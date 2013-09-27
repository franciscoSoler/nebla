/*
 * Programa: Ejemplo de server CONCURRENTE usando protocolo TCP (tcpservc).
 * Descripcion:	
 *	Hacer un open pasivo (tcpoppas).
 *	Esperar por un cliente.
 *	Por cada cliente lanza un programa separado (tcpecos) para :
 *		Recibir un mensaje
 *		Devolver el mensaje al cliente anteponiendo la hora de recepci�n
 *	Para terminarlo (kill -9 nro de proceso)
 * Sintaxis: tcpserv [port]
 *	port: 	port sobre el cual atiende este servidor
 *	Si no se ingresa nro de port: se usa el PORT_FIJO (ver inet.h)
 * Usa:		Procedimiento tcpservertrans() (en tcpecos), 
                           Funcion tcpoppas().
 */

#include	"inet.h"
extern int tcpoppas(int);


int main(int argc, char *argv[])

{
 int			sockfd,		/* socket que sirve como template */ 
                        newsockfd;	        /* socket conectado al cliente */
 unsigned		clilen,	        /* longitud dir. cliente */
                        childpid;	        /* pid del hijo */
 struct sockaddr_in	cli_addr;
 char mostrar[120];                      /* mensaje para mostrar en pantalla */
char	*pname;         /* nombre del programa */
int 	port;	              /* port de conexion */

 static char el_socket[15];         /* string que contiene el socket para el servidor de eco */
 pname = argv[0];
/*
 *		Verificar si se informo un port o se debe usar el default
 */
 if (argc > 1)
	port = atoi(argv[1]);	/* convertir port ingresado a binario */
 else
	port = PORT_FIJO;	/* usar el port por default (ver inet.h) *//*
/*			INICIALIZACION DEL SERVER
 * 				OPEN PASIVO
 */
 if ( (sockfd = tcpoppas(port)) < 0)
	{
	 perror("server: no se puede abrir el stream socket");
	 exit(1);
	 }
  sprintf (mostrar,"server: se hizo el open pasivo, socket %d\n", sockfd);
  write(fileno(stdout), mostrar, strlen(mostrar));  

 
 /*
  *			PROCESAMIENTO DEL SERVER
  */
 for ( ; ; ) {	/* no termina nunca */
 /*
  * 		ESTE ES UN EJEMPLO DE UN SERVER CONCURRENTE
  * 	Espera por un cliente en el accept:
  *        Cuando llega un cliente crea el hijo que lo atiende
  *        y el padre vuelve al accept por otro cliente.
  */
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
	       {
       		perror ("server: error en el accept");
         	exit(1); 
         	}
	if ( (childpid = fork()) < 0)
		{ 
		perror("server: error en el fork");
		exit(1);
		}

	else if (childpid == 0)
		{	 
/*
 *	 	 PROCESO HIJO (child) que atiende al cliente
 */
		close(sockfd);		/* cerrar socket original */
		sprintf (mostrar,"server: socket armado con un cliente  %d\n", newsockfd);
		write(fileno(stdout), mostrar, strlen(mostrar));  
		sprintf(el_socket, "%d\n",newsockfd); /* pasarle el socket al hijo que atiende */
		/* 
		*		se lanza el proceso que atiende a ese cliente
		 */
		execlp("./tcpecos", "tcpecos", el_socket, (char *)0);
		perror("server: error al lanzar el servidor de eco");
		exit(3);
	         }
 /*
  *		 PROCESO PADRE, se prepara para recibir otro cliente
  */
	close(newsockfd);		/* cerrar el socket pasado al hijo */
	}
}

