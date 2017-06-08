#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/message_wrapper.h"


int main(int argc, char** argv) {
	pid_t pid = getpid();

	Logger log = crearLogger();

	if (argc != 2){
		printf("Deberia recibir por parametro abrir, cerrar o un numero especificando cuantos clientes lanzar\n");
	}

	if (strcmp(argv[1],"abrir") == 0 || strcmp(argv[1],"cerrar") == 0) {
		int id = getmsgq(MSGQ_RECIBIR_HELADERO);

		if (id == -1){
			id = getmsgq(MSGQ_RECIBIR_CAJERO);
		}

		if (id == -1){
			id = getmsgq(MSGQ_RECIBIR_CLIENTE);
		}

		if (id == -1){
			perror("No hay ningun mom abierto, no se puede mandar mensaje de abrir o cerrar heladeria\n");
			return -1;
		}

		MessageQ msg;
		msg.mtype = 1;

		if(strcmp(argv[1],"abrir") == 0){
			sprintf(msg.type,"%d",MSG_BROKER_ABRIR_HELADERIA);
			enviarmsgq(id,&msg,sizeof(MessageQ));
			escribirLog(&log,DEBUG,pid,MANAGER_NAME,"Se vuelve a abrir la heladeria");
		} else {
			sprintf(msg.type,"%d",MSG_BROKER_CERRAR_HELADERIA);
			enviarmsgq(id,&msg,sizeof(MessageQ));
			escribirLog(&log,DEBUG,pid,MANAGER_NAME,"Se cierra la heladeria");
		}

	} else {
		char buffer[BUFFER_SIZE];
		
		sprintf(buffer,"Voy a lanzar %s clientes",argv[1]);
		escribirLog(&log,DEBUG,pid,MANAGER_NAME,buffer);

		int clientes = atoi(argv[1]);
		int i;

		for (i = 0; i < clientes; i++){
			if (fork() == 0) {
				execl("./cliente","./cliente",(char*) NULL);
				perror("Exec fallo");
				return -1;
			}
		}
	}

	cerrarLogger(&log);
}

