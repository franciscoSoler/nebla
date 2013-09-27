#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <signal.h>
#include <stdio.h>

#include "EventHandler.h"
#include "../common/Object.h"

/**
 * Clase encargada de guardar los handlers de todas las seniales
 * definidas. Como la mascara de seniales es unica para el proceso
 * esta clase debe ser un singleton.
 */
class SignalHandler:public Object
{
	private:
		static SignalHandler* m_instance;
		EventHandler* m_signalHandlers [ NSIG ];

		SignalHandler ( void );
		static void dispatcher ( int signum );

	public:
		/**
		 * Obtiene la instancia, si no existe la crea
		 */
		static SignalHandler* getInstance ();

		/**
		 * Destruye la instancia
		 */
		static void destroy ();

		/**
		 *
		 */
		EventHandler* registerHandler ( int signum,EventHandler* eh );

		/**
		 *
		 */
		int removeHandler ( int signum );

		/**
		 * Bloquea una senial
		 */
		static void blockSignal(int signum);

		/**
		 * Desbloquea una senial
		 */
		static void unblockSignal(int signum);

};

#endif /* SIGNALHANDLER_H_ */
