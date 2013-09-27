#ifndef MESSAGE_H_
#define MESSAGE_H_

#define	REQUEST	    1
#define	REPLY	    2
#define TEXT_SIZE	255

/*
 * Esto es un ejemplo de como armar una estructura tipo "mensaje" para poner en la cola de mensajes
 */

typedef struct message
{
	long mtype;             // Esto nunca hay q olvidarse de ponerlo!
	int id;                 // A partir de aca ponemos los campos que queremos
	char text[TEXT_SIZE];
} message;

#endif /* MESSAGE_H_ */
