#include "SignalHandler.h"

typedef struct SignalHandler {

		SIG_Trap* signal_handlers [ NSIG ];

} SignalHandler;

// el signal handler es un singleton porque nunca va a haber mas de uno por proceso
static SignalHandler _SignalHandlerInstance;

/*SignalHandler* SignalHandlerGetInstance () {

	if ( instance == NULL )
		instance = new SignalHandler;

	return instance;
}

void SignalHandlerDestroy () {
	if ( instance != NULL ) {
		delete ( instance );
		instance = NULL;
	}
}*/

void _SignalHandlerDispatcher ( int signum ) {
	//SignalHandler* handler = SignalHandlerGetInstance();
	if ( _SignalHandlerInstance.signal_handlers [ signum ] != 0 )
		SIG_TrapHandleSignal(_SignalHandlerInstance.signal_handlers [ signum ], signum );
}

SIG_Trap* SignalHandlerRegisterHandler ( int signum,SIG_Trap* eh ) {

	//SignalHandler* handler = SignalHandlerGetInstance();
	SIG_Trap* old_sigtrap = _SignalHandlerInstance.signal_handlers [ signum ];
	_SignalHandlerInstance.signal_handlers [ signum ] = eh;

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = _SignalHandlerDispatcher;
	sigemptyset ( &sa.sa_mask );	// inicializa la mascara de seniales a bloquear durante la ejecucion del handler como vacio
	sigaddset ( &sa.sa_mask,signum );
	sigaction ( signum,&sa,0 );	// cambiar accion de la senial

	return old_sigtrap;
}

int SignalHandlerRemoveHandler ( int signum ) {
	//SignalHandler* handler = SignalHandlerGetInstance();
	_SignalHandlerInstance.signal_handlers [ signum ] = NULL;
	return 0;
}
