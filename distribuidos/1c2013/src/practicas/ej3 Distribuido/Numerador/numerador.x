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
			 
program NUMERADOR {
	version NUMERADOR2 {
		retorno NUMERADORCLIENTE() = 1;
		retorno NUMERADORVENDEDOR() = 2;

		void DEVOLVERCLIENTE(int) = 3;
		void DEVOLVERVENDEDOR(int) = 4;

		void RESETEARCLIENTES() = 5;
		void RESETEARVENDEDORES() = 6;

		retorno LISTADOCLIENTES() = 7;
		retorno LISTADOVENDEDORES() = 8;
	} = 2;
} = 0x20000099;
