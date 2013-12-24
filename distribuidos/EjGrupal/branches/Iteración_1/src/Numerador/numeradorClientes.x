/* 
 * estructura de la devolucion de resultados de la invocacion a la subrutina en RPC 
 * ESTE TIPO DE ESTRUCTURAS SON OBLIGATORIAS SI HAY QUE DEVOLVER RESULTADOS
 */

union retorno 
      switch (int cod_ret) {
             case 1: int numero;
             case 2: char informacion[800];
             default: void;
             };
			 
program NUMERADORCLIENTE {
	version NUMERADORCLIENTE1 {
		retorno OBTENERIDCLIENTE() = 1;
		void DEVOLVERIDCLIENTE(int) = 2;
		void RESETEARCLIENTES() = 3;
		retorno LISTADOCLIENTES() = 4;
	} = 1;
} = 0x20000099;
