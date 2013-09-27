/*
 * msg_iv.x: Ejemplo de log remoto de mensajes y devolucion de un texto 
 */

/* 
 * estructura de la devolucion de resultados de la invocacion a la subrutina en RPC 
 * ESTE TIPO DE ESTRUCTURAS SON OBLIGATORIAS SI HAY QUE DEVOLVER RESULTADOS
 */

union retorno 
      switch (int cod_ret) {
             case 1: char message[800];
             default: void;
             };
			 
program MESSAGEPROG {
	version MESSAGEVERS {
		retorno PRINTMESSAGE(string<>) = 1;
	} = 2;
} = 0x20000099;
