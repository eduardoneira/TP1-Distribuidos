#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/message_wrapper.h"
#include "../include/mensaje_gustos.h"


int main(int argc, char** argv) {
	pid_t pid = getpid();

	Logger log = crearLogger();

	if (argc < 2){
		printf("Deberia recibir por parametro abrir, cerrar, lanzar [cant_cliente] o destruir [tipo] [momId de cajero o heladero] \n");
		cerrarLogger(&log);
		return 1;
	}

	if (strcmp(argv[1],"abrir") == 0 || strcmp(argv[1],"cerrar") == 0) {
		int id = getmsgq(MSGQ_BROKER_IN_ROUTER);

		if (id == -1){
			id = getmsgq(MSGQ_RECIBIR_HELADERO);
		}

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

	} else if (strcmp(argv[1],"destruir") == 0){
		if (argc != 4) {
			printf("Falta el tipo y/o el numero de momId a destruir\n");
			cerrarLogger(&log);
			return 1;
		}

		int msgq =  -1;

		if (strcmp(argv[2],HELADERO) == 0){
			msgq = getmsgq(MSGQ_POR_MOMID_HELADERO);
		} else if (strcmp(argv[2],CAJERO) == 0){
			msgq = getmsgq(MSGQ_POR_MOMID_CAJERO);
		}

		if (msgq != -1 ){
			int momId = atoi(argv[3]);
			Mensaje_gustos msg_gustos;
			crearMsgIrse(&msg_gustos);

			MessageQ msg;
			strcpy(msg.type,"0");
			msg.mtype = momId;
			serializeMsgGusto(&msg_gustos,msg.payload);
			enviarmsgq(msgq,&msg,sizeof(MessageQ));
		}


	} else if (strcmp(argv[1],"lanzar") == 0){
		if (argc != 3) {
			printf("Falta el numero de clientes a lanzar\n");
			cerrarLogger(&log);
			return 1;
		}
		char buffer[BUFFER_SIZE];
		
		sprintf(buffer,"Voy a lanzar %s clientes",argv[1]);
		escribirLog(&log,DEBUG,pid,MANAGER_NAME,buffer);

		int clientes = atoi(argv[2]);
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

