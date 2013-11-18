/*
 * Funcion enviar()
 *	Escribe n bytes sobre un descriptor:
 *		si no se escribieron n bytes
 *			repetir hasta que se hayan escrito los n bytes
 *	(Se debe usar esta funcion cuando el descriptor es un stream socket 
 * Sintaxis: int enviar(sockfd, void *datos, size_t nbytes)
             sockfd: descriptor del socket a usar .
	    datos: datos a enviar.
	    nbytes: cantidad datos a enviar (en bytes).
 */
#include "inet.h"

int enviar(int sockfd, void *datos, size_t nbytes)
{
	/* Variables. */
	int enviados;			/* Cantidad de bytes enviados. */
	int ult_envio;			/* Cantidad de bytes enviados en el último envío. */
	
	/* Envía los datos. */
	enviados = 0;
	while(nbytes)
	{
        ult_envio = write(sockfd, ((unsigned char *) datos) + enviados, nbytes);
		if (ult_envio <= 0) return enviados;    /* error */
		enviados += ult_envio;
		nbytes -= ult_envio;
	}

	/* Devuelve la cantidad de bytes enviados. */
	return enviados;
}
