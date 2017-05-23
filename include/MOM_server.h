#ifndef _MOM_CAJERO_HELADERO_
#define _MOM_CAJERO_HELADERO_

#include "msg_queue.h"
#include "mensaje_gustos.h"
#include "mensaje_helado.h"
#include "mensaje_ticket.h"
#include "socket.h"
#include <stdbool.h>
#include <vector>

typedef struct MOM_handler {
	int _id_recibir_mensaje;
	int _id_enviar_mensaje;
	char hosts[CANT_TIPOS_PROCESOS][20];
	bool _socket_leer; 				// true si leo de un socket, sino soy escritor
} MOM_handler;

int _get_id_receptor(char* emisor, char* receptor) {
	if (strcmp(emisor,CLIENTE) == 0 && strcmp(receptor,CAJERO) == 0) {
		return MSGQ_PASAMANOS_CLIENTE_MOM_PEDIDO;
	} else if (strcmp(emisor,CAJERO) == 0 && strcmp(receptor,HELADERO) == 0) {
		return MSGQ_PASAMANOS_CAJERO_MOM_PEDIDO;
	} else if (strcmp(emisor,CAJERO) == 0 && strcmp(receptor,CLIENTE) == 0) {
		return MSGQ_PASAMANOS_CAJERO_MOM_TICKET;
	} else if (strcmp(emisor,HELADERO) == 0 && strcmp(receptor,CLIENTE) == 0) {
		return MSGQ_PASAMANOS_HELADERO_MOM_HELADO;
	} else  {
		return -1;
	}
}

int _get_id_emisor(char* emisor, char* receptor) {
	if (strcmp(emisor,CLIENTE) == 0 && strcmp(receptor,CAJERO) == 0) {
		return MSGQ_PASAMANOS_MOM_CAJERO_PEDIDO;
	} else if (strcmp(emisor,CAJERO) == 0 && strcmp(receptor,HELADERO) == 0) {
		return MSGQ_PASAMANOS_MOM_HELADERO_PEDIDO;
	} else if (strcmp(emisor,CAJERO) == 0 && strcmp(receptor,CLIENTE) == 0) {
		return MSGQ_PASAMANOS_MOM_CLIENTE_TICKET;
	} else if (strcmp(emisor,HELADERO) == 0 && strcmp(receptor,CLIENTE) == 0) {
		return MSGQ_PASAMANOS_MOM_CLIENTE_HELADO;
	} else  {
		return -1;
	}
}

int _getNumHost(char* host){
	if (strcmp(host,CAJERO) == 0) {
		return 0;
	} else if (strcmp(host,HELADERO) == 0) {
		return 1;
	} else if (strcmp(host,CLIENTE) == 0) {
		return 2;
	} else if (strcmp(host,RPC) == 0) {
		return 3;
	} else {
		return -1;
	}
}

MOM_handler abrirMOM(char* emisor, char* receptor,char* quien_soy, int puerto) {
	MOM_handler handler;
    //TODO:DESPUES SACAR ESTOS
    FILE* fd = fopen(IPS,"r");
    char nombre[20];
    char ip[20];

    for(int i = 0; i < CANT_TIPOS_PROCESOS; i++){
        fscanf(fd,"%s %s\n",nombre,ip);
        strcpy(handler.hosts[_getNumHost(nombre)],ip);
    }
    fclose(fd);

	if (strcmp(quien_soy,emisor) == 0) {
		handler._socket_leer = false;
		handler._id_recibir_mensaje = getmsgq(_get_id_receptor(emisor,receptor));
		handler._id_enviar_mensaje = abrir_socket_activo(handler.hosts[_getNumHost(receptor)],puerto);
		if (handler._id_enviar_mensaje == -1) {
			exit(-1);
		}
	} else {
		handler._socket_leer = true;
		handler._id_recibir_mensaje = abrir_socket_pasivo(handler.hosts[_getNumHost(emisor)],puerto);
		if (handler._id_recibir_mensaje == -1) {
			exit(-1);
		}
		handler._id_enviar_mensaje = getmsgq(_get_id_emisor(emisor,receptor));
	};

	return handler;
}

int recibirMsg(MOM_handler* handler, void* msg,size_t size) {
	if (handler->_socket_leer){
		return leer_socket(handler->_id_recibir_mensaje,msg,size);
	} else {
		return recibirmsgqSinCheckeo(handler->_id_recibir_mensaje,msg,size,0);
	}

}

void enviarMsg(MOM_handler* handler,void* msg, size_t size) {
	if (handler->_socket_leer){
		enviarmsgq(handler->_id_enviar_mensaje,msg,size);
	} else {
		escribir_socket(handler->_id_recibir_mensaje,msg,size);
	}

}


void cerrarMOM(MOM_handler* handler) {
	if (handler->_socket_leer){
		cerrar_socket(handler->_id_enviar_mensaje);
	} else {
		cerrar_socket(handler->_id_recibir_mensaje);
	}

	handler->_id_recibir_mensaje = -1;
	handler->_id_enviar_mensaje = -1;
}





#endif