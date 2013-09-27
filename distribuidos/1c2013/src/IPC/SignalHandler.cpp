#include "SignalHandler.h"
#include <string.h>
#include "../common/Exception.h"

SignalHandler* SignalHandler :: m_instance = NULL;

SignalHandler :: SignalHandler ()
{
}

SignalHandler* SignalHandler :: getInstance ()
{

	if ( m_instance == NULL )
		m_instance = new SignalHandler ();

	return m_instance;
}

void SignalHandler :: destroy ()
{
	if ( m_instance != NULL )
	{
		delete ( m_instance );
		m_instance = NULL;
	}
}

EventHandler* SignalHandler :: registerHandler ( int signum,EventHandler* eh )
{

	EventHandler* old_eh = SignalHandler :: m_signalHandlers [ signum ];
	SignalHandler :: m_signalHandlers [ signum ] = eh;

	struct sigaction sa;
	sa.sa_handler = SignalHandler :: dispatcher;

	// inicializa la mascara de seniales a bloquear
	// bloqueando la senial actual.
	if(sigemptyset ( &sa.sa_mask )<0 )
	{
		Exception e("Error while initializing the signal mask");
		throw e;
	}

	if(sigaddset( &sa.sa_mask, signum) <0)
	{
		Exception e("Error while adding the signal to the mask");
		throw e;
	}

	sa.sa_flags = 0;
	sigaction ( signum,&sa,0 );	// cambiar accion de la senial

	return old_eh;
}

void SignalHandler :: dispatcher ( int signum )
{
	if ( getInstance()->m_signalHandlers [ signum ] != 0 )
	    getInstance()->m_signalHandlers [ signum ]->handleSignal ( signum );
}

int SignalHandler :: removeHandler ( int signum )
{
	SignalHandler :: m_signalHandlers [ signum ] = NULL;
	return 0;
}

void SignalHandler::blockSignal(int signum)
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));

	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, signum);
	sigaction(signum, &act, NULL);
}

void SignalHandler::unblockSignal(int signum)
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));

	act.sa_handler = SIG_DFL;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, signum);
	sigaction(signum, &act, NULL);
}
