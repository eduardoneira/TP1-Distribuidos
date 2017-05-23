#include <stdbool.h>
#include "../include/MOM_server.h"

/**
 * Argumentos esperados:
 * char* 	quien_envia
 * char* 	quien_recibe
 * size_t	tamanio_mensaje
 * char*    quien_soy
 * char*    si soy socket activo o pasivo
 * int      puerto
 * */

int main(int argc, char** argv){
	Logger log = crearLogger();
	pid_t pid = getpid();
	char buffer[64];

	sprintf(buffer,"Hola soy el MOM %s-%s con size %s siendo %s con un socket %s con puerto %s", argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
	escribirLog(&log,TRACE,pid,MOM,buffer);

	MOM_handler handler = abrirMOM(argv[1],argv[2],argv[4],argv[5],atoi(argv[6]));

	size_t size = atoi(argv[3]);
	void* msg = malloc(size);

	bool termine = false;

	while(!termine){
		if (recibirMsg(&handler,msg,size) == -1) {
			termine = true;
		} else {
			escribirLog(&log,TRACE,pid,MOM,"Recibi un msg :p");
			enviarMsg(&handler,msg,size);
			escribirLog(&log,TRACE,pid,MOM,"Envie un msg :p");
		}
	}

	free(msg);
	cerrarMOM(&handler);

    sprintf(buffer,"Cerrando MOM de %s-%s de %s", argv[1],argv[2],argv[4]);
    escribirLog(&log,TRACE,pid,MOM,buffer);
	cerrarLogger(&log);

	return 0;
}