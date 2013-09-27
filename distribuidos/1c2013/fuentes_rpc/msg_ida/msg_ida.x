/*
 * msg_ida.x: Ejemplo de env’o de un mensaje con  codigo de retorno solamente
 */
program MESSAGEPROG {
	version MESSAGEVERS {
		int PRINTMESSAGE(string) = 1;
	} = 1;
} = 0x20000099;
