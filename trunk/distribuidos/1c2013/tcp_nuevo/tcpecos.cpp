/*
 * Programa: Ejemplo de server CONCURRENTE usando protocolo TCP (tcpservc).
 *                    Programa que atiende a un cliente en particular (tcpecos).
 * Descripcion:	
 *		Recibir un mensaje
 *		Devolver el mensaje al cliente anteponiendo la hora de recepci�n
 *	
 * Usa:		Procedimiento tcpservertrans().
 */

#include	"inet.h"
extern void tcpservertrans(int, char *, pid_t);

int main(int argc, char *argv[])

{
 int		    newsockfd;	        /* socket conectado al cliente */
 pid_t               pid_eco;	        /* pid del eco que atiende al cliente */
 char mostrar[120];
char	*pname;         /* nombre del programa */
int 	port;	              /* port de conexion */
                      /* mensaje para mostrar en pantalla */
 pname = argv[0];
/*
 *		Verificar si se pasa el descriptor del socket
 */
 if (argc > 1)
	newsockfd = atoi(argv[1]);	/* convertir port ingresado a binario */
 else
	{
	 perror("tcpecos: error en el socket al invocar este programa");
	 exit(1);
	 }
  pid_eco = getpid();
  sprintf (mostrar,"%s (%d): atiendo al cliente en el descriptor %d\n ", pname, pid_eco, newsockfd);
  write(fileno(stdout), mostrar, strlen(mostrar));  
  tcpservertrans(newsockfd, pname, pid_eco);	/* procesar el pedido */
  close(newsockfd);
  exit(0);
}
