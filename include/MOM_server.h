#ifndef _MOM_CAJERO_HELADERO_
#define _MOM_CAJERO_HELADERO_

#include "msg_queue.h"
#include "mensaje_gustos.h"
#include "mensaje_helado.h"
#include "mensaje_ticket.h"
#include "socket.h"
#include "message_wrapper.h"
#include "mensaje_registro.h"
#include <stdbool.h>

typedef struct MOM_handler {
	int _socket;
	int _id_cola_recibir;
	int _id_cola_pid;
	int _id_cola_ticket;
	int _id_cola_momId;
	bool socket_leer; 		// true si leo de un socket, sino soy escritor
} MOM_handler;

bool abrirMOM(MOM_handler* handler,char* quien_soy) {

	int fd = abrir_socket_activo(IP_BROKER,PORT_BROKER);

	if (fd == -1) {
		perror("Acordarse que hay lanzar al broker primero\n");
		return true;
	}

	if (strcmp(quien_soy,CLIENTE) == 0) {
		handler->_id_cola_recibir = getmsgq(MSGQ_RECIBIR_CLIENTE);
	} else if (strcmp(quien_soy,CAJERO) == 0){
		handler->_id_cola_recibir = getmsgq(MSGQ_RECIBIR_CAJERO);
	} else if (strcmp(quien_soy,HELADERO) == 0){
		handler->_id_cola_recibir = getmsgq(MSGQ_RECIBIR_HELADERO);
	}

	handler->_socket = fd;
	handler->_id_cola_momId = getmsgq(MSGQ_POR_MOMID);
	handler->_id_cola_pid = getmsgq(MSGQ_POR_PID);
	handler->_id_cola_ticket = getmsgq(MSGQ_POR_TICKET);

	if (fork() == 0){ 						//hijo lee del socket
		handler->socket_leer = true;
	} else { 								//padre escribe el socket
		handler->socket_leer = false;
	}

	return false;
}

int recibirMsg(MOM_handler* handler,Message* msg) {
	if (handler->socket_leer){
		return leer_socket(handler->_socket,msg,sizeof(Message));
	} else {
		MessageQ msgq;
		int ret = recibirmsgqSinCheckeo(handler->_id_cola_recibir,&msgq,sizeof(MessageQ),0);
		crearMessage(msg,&msgq);
		return ret;
	}

}

void enviarMsg(MOM_handler* handler,Message* msg) {
	if (handler->socket_leer){
		MessageQ msgq;
		crearMessageQ(msg,&msgq,getMtype(*msg));
		int type = atoi(msg->type);

		if (type == MSG_BROKER_REGISTER){
			enviarmsgq(handler->_id_cola_pid,&msgq,sizeof(MessageQ));
		} else if (type == MSG_BROKER_HELADO){
			enviarmsgq(handler->_id_cola_ticket,&msgq,sizeof(MessageQ));
		} else {
			enviarmsgq(handler->_id_cola_momId,&msgq,sizeof(MessageQ));
		}
	} else {
		escribir_socket(handler->_socket,msg,sizeof(Message));
	}

}

void cerrarMOM(MOM_handler* handler) {
	cerrar_socket(handler->_socket);
}

#endif