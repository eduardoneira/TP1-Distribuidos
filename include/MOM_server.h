#ifndef _MOM_CAJERO_HELADERO_
#define _MOM_CAJERO_HELADERO_

#include "msg_queue.h"
#include "mensaje_gustos.h"
#include "mensaje_helado.h"
#include "mensaje_ticket.h"

typedef struct MOM_handler {
	int _id_recibir_mensaje;
	int _id_enviar_mensaje;
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

MOM_handler abrirMOM(char* emisor, char* receptor) {
	MOM_handler handler;
	handler._id_recibir_mensaje = getmsgq(_get_id_receptor(emisor,receptor));
	handler._id_enviar_mensaje = getmsgq(_get_id_emisor(emisor,receptor));
	return handler;
}

int recibirMsg(MOM_handler* handler, void* msg,size_t size) {
	return recibirmsgqSinCheckeo(handler->_id_recibir_mensaje,msg,size,0);
}

void enviarMsg(MOM_handler* handler,void* msg, size_t size) {
	enviarmsgq(handler->_id_enviar_mensaje,msg,size);
}


void cerrarMOM(MOM_handler* handler) {
	handler->_id_recibir_mensaje = -1;
	handler->_id_enviar_mensaje = -1;
}

#endif