#ifndef _MOM_CAJERO_HELADERO_
#define _MOM_CAJERO_HELADERO_

#include "msg_queue.h"
#include "mensaje_gustos.h"

typedef struct MOM_Cajero_Heladero_handler {
	int _msgq_id_CM;
	int _msgq_id_MH;
} MOM_Cajero_Heladero_handler;

MOM_Cajero_Heladero_handler abrirMOM() {
	MOM_Cajero_Heladero_handler handler;
	return handler;
}

void recibirMsg(MOM_Cajero_Heladero_handler* handler, Mensaje_gustos* msg) {
	recibirmsgq(handler->_msgq_id_CM,msg,sizeof(Mensaje_gustos),0);
}

void enviarMsg(MOM_Cajero_Heladero_handler* handler,Mensaje_gustos* msg) {
	enviarmsgq(handler->_msgq_id_MH,msg,sizeof(Mensaje_gustos));
}

void cerrarMOM(MOM_Cajero_Heladero_handler* handler) {
	handler->_msgq_id_MH = -1;
	handler->_msgq_id_MH = -1;	
}

#endif