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
			 
program CESARPROG {
	version CESARVERS2 {
		retorno CODCESAR(string<>) = 1;
		retorno DECODCESAR(string<>) = 2;
	} = 2;
	version CESARVERS3 {
		retorno CODCESAR(string<>) = 1;
		retorno DECODCESAR(string<>) = 2;
	} = 3;

} = 0x20000099;
