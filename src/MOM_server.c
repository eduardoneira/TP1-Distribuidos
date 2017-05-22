#include "../include/MOM_server.h"
#include "../include/SignalHandler.h"

/**
 * Argumentos esperados:
 * char* 	quien_envia
 * char* 	quien_recibe
 * size_t	tamanio_mensaje
 * */

int main(int argc, char** argv){
	Logger log = crearLogger();
	pid_t pid = getpid();
	char buffer[64];

	sprintf(buffer,"Hola soy el MOM %s-%s con size %s", argv[1],argv[2],argv[3]);
	escribirLog(&log,TRACE,pid,MOM,buffer);

	SIG_Trap SIGINT_trap;
	SIG_TrapInit(&SIGINT_trap, SIGINT);

	MOM_handler handler = abrirMOM(argv[1],argv[2]);

	size_t size = atoi(argv[3]);
	void* msg = malloc(size);

	SignalHandlerRegisterHandler(SIGINT, &SIGINT_trap);

	while(SIG_TrapSignalWasReceived(&SIGINT_trap) == 0){
		recibirMsg(&handler,msg,size);
		escribirLog(&log,TRACE,pid,MOM,"Recibi un msg :p");
		enviarMsg(&handler,msg,size);
	}

	free(msg);
	cerrarMOM(&handler);

    sprintf(buffer,"Cerrando MOM de %s-%s", argv[1],argv[2]);
    escribirLog(&log,TRACE,pid,MOM,buffer);
	cerrarLogger(&log);

	return 0;
}