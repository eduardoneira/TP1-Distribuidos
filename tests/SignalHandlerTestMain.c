#include <iostream>

#include "SignalHandler.h"
#include "SIG_Trap.h"

int main()
{
	// el SignalHandler se guarda una referencia al SIG_Trap, por lo que hay que asegurarse que
	// el SIG_Trap no se destruya hasta que se cierre el proceso (o se desregistre del handler)
	SIG_Trap SIGINT_trap;
	SIG_TrapInit(&SIGINT_trap, SIGINT);

	SignalHandlerRegisterHandler(SIGINT, &SIGINT_trap);
	while(SIG_TrapSignalWasReceived(&SIGINT_trap) == 0){
		;
	}

	std::cout << "Me mandaron SIGINT: " << SIG_TrapSignalWasReceived(&SIGINT_trap) << std::endl;
	return 0;
}
