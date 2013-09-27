/*
 * Programa: Ejemplo de cliente usando protocolo TCP (es un ping en TCP): (tcpcli)
 * Descripcion:
 *	Verificar parametros ingresados
 *	Hacer un open activo (tcpopact)
 *      Repetir hasta que el usuario ingrese EOF
 *		El usuario ingresa un mensaje por teclado
 *		El proceso lo envia al servidor.
 *		El servidor lo devuelve.
 *		El proceso cliente muestra el mensaje en pantalla.
 * Sintaxis: tcpcli [ servidor [port]]
 *	servidor: 	nombre oficial del host server
 *	port:		nro de port
 *	Ambos parametros son opcionales: 
 *		Si no se especifica port, se usa el PORT_FIJO (ver inet.h)
 *		Si no se especifica host, se usa el localhost 
 * Usa:	Funcion tcpopact(), 
 *               Procedimientos tcpclienttrans(), close()
 */

#include	"inet.h"
extern int tcpopact(char *, int);
extern void tcpclienttrans (int , char *, pid_t );

int main(int argc, char *argv[])
{
 int			sockfd, codigo_retorno;
 char			*server;     /* nombre del server */
 char localhost[] =	"localhost"; /* nombre del server x defecto */	
 char mostrar[120];
 char	*pname;         /* nombre del programa */
 int 	port;	              /* port de conexion */

 pid_t pid_cl;               /* pid del cliente */
 pname = argv[0];	/* programa cliente */
 
 /* mostrar el pid del cliente */
 
 pid_cl = getpid();
 sprintf(mostrar,"%s Cliente con Pid =%d\n", pname, pid_cl);
 write(fileno(stdout),mostrar,strlen(mostrar));
 /*			
  *			Verificar nombre server y port
 /*
  * 	Paso 1: Verificar si se ingreso un numero de port o se usa el default
  */
  if (argc > 2) 
	port = atoi(argv[2]);	/* se especifico port: convertir a binario */
  else
	port = PORT_FIJO;	/* usar el port por defecto (ver inet.h) */
  if ( ! (port > 0))			/* validarlo */
	{			/* valor incorrecto: aviso y terminar */
	sprintf(mostrar,"%s (%d): Nro. de port invalido %s\n", pname, pid_cl, argv[2]);
        write(fileno(stdout),mostrar,strlen(mostrar));
exit(1);
	}
 /*
  *	Paso 2: Verificar si se ingreso nombre del server, o se usa el 
  *	        localhost 
  */
  if (argc > 1)
	server = argv[1];	/* se especifico nombre de server */
  else
	server = localhost;	/* ver inet.h */
 /*
  *	Paso 3: Hacer un open activo y obtener el socket.
  */
  sockfd = tcpopact(server, port); 
  if ( sockfd < 0 )
	{		/* Hay un error: aviso y terminar */
	if (sockfd == -2) 
	    {
	    sprintf(mostrar,"%s (%d): Nombre de server no existe %s\n", pname, pid_cl, server);
            write(fileno(stdout), mostrar, strlen(mostrar));
	    exit(1);
	    }
         else
	    {
	    perror("error en el open activo");
	    exit(1);
	    }
 }
 /*
  *		PROCESO DEL CLIENTE (hasta que el usuario ingrese EOF)
  */
 tcpclienttrans(sockfd, pname, pid_cl);	/* transferencia de datos sobre la conexion*/

 /*
  *		CERRAR LA CONEXION Y TERMINAR
  */
 close(sockfd);    
 exit(0);
}
