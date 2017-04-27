#include "../include/MOM_cajero_heladero.h"
#include "../include/SignalHandler.h"
#include "../include/SIG_Trap.h"

//TODO: agregar logeo

int main(int argc, char** argv){
	
	SIG_Trap SIGINT_trap;
	SIG_TrapInit(&SIGINT_trap, SIGINT);

	MOM_Cajero_Heladero_handler handler = abrirMOM();
	Mensaje_gustos msg;

	SignalHandlerRegisterHandler(SIGINT, &SIGINT_trap);
	while(SIG_TrapSignalWasReceived(&SIGINT_trap) == 0){
		recibirMsg(&handler,&msg);
		enviarMsg(&handler,&msg);
	}

	cerrarMOM(&handler);

	return 0;
}