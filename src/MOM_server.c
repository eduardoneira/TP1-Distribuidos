#include <stdbool.h>
#include "../include/MOM_server.h"

/**
 * Argumentos esperados:
 * char*    quien_soy
 * */

int main(int argc, char** argv){
	Logger log = crearLogger();
	char buffer[128];

	MOM_handler handler;
	bool termine = abrirMOM(&handler,argv[1]);

	pid_t pid = getpid();
	if (handler.socket_leer){
		sprintf(buffer,"Hola soy el MOM para %s que lee del socket", argv[1]);
	} else {
		sprintf(buffer,"Hola soy el MOM para %s que escribe al socket", argv[1]);
	}
	escribirLog(&log,TRACE,pid,MOM,buffer);

	Message msg;

	while(!termine){
		if (recibirMsg(&handler,&msg) == -1) {
			termine = true;
		} else {
			sprintf(buffer,"Recibi un msg. TYPE:%s, PAYLOAD: %s",msg.type,msg.payload);
			escribirLog(&log,TRACE,pid,MOM,buffer);
			enviarMsg(&handler,&msg);
			escribirLog(&log,TRACE,pid,MOM,"Envie un msg :p");
		}
	}

	cerrarMOM(&handler);

    sprintf(buffer,"Cerrando MOM para %s", argv[1]);
    escribirLog(&log,TRACE,pid,MOM,buffer);
	cerrarLogger(&log);

	return 0;
}