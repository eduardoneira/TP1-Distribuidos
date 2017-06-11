#ifndef TPS_DISTRIBUIDOS_ICLIENTEMOM_H
#define TPS_DISTRIBUIDOS_ICLIENTEMOM_H

#include "cliente_handler.h"
#include "msg_queue.h"
#include "memoria_compartida.h"
#include "semaforo.h"
#include "mensaje_gustos.h"
#include "mensaje_ticket.h"
#include "mensaje_helado.h"
#include "mensaje_registro.h"
#include "estado_heladeria.h"
#include "mensaje_bool.h"
#include "message_wrapper.h"

Cliente_handler registrarCliente() {
	
	Cliente_handler handler;

	handler._msgq_id_recibir_momId = getmsgq(MSGQ_POR_MOMID_CLIENTE);
	handler._msgq_id_enviar= getmsgq(MSGQ_RECIBIR_CLIENTE);
	handler._msgq_id_recibir_ticket = getmsgq(MSGQ_POR_TICKET);

	handler.id = registrarse(handler._msgq_id_enviar,CLIENTE);

	return handler;
}

void desregistrarCliente(Cliente_handler* handler) {
	desregistrarse(handler->_msgq_id_enviar,handler->id,CLIENTE);
}

bool checkearEstado(Cliente_handler* handler, int tipo_msg) {
	Mensaje_bool msg_bool = crearMensajeBool(handler->id,false);

	MessageQ msg;
	msg.mtype = 1;
	sprintf(msg.type,"%d",tipo_msg);
	serializeMsgBool(&msg_bool,msg.payload);

	enviarmsgq(handler->_msgq_id_enviar,&msg,sizeof(MessageQ));
	recibirmsgqSinCheckeo(handler->_msgq_id_recibir_momId,&msg,sizeof(MessageQ),handler->id);

	deserializeMsgBool(&msg_bool,msg.payload);

	return msg_bool.estado;
}

bool clienteEntrarEnLaHeladeria(Cliente_handler* handler) {
	return checkearEstado(handler,MSG_BROKER_PUEDO_ENTRAR);
}

bool clienteEntrarEnLaCola(Cliente_handler* handler) {
	return checkearEstado(handler,MSG_BROKER_HAY_LUGAR_COLA);
}

void clienteSalirDeLaCola(Cliente_handler* handler) {
	checkearEstado(handler,MSG_BROKER_SALIR_COLA);
}

bool clienteReservarLugarParaSentarse(Cliente_handler* handler) {
	return checkearEstado(handler,MSG_BROKER_HAY_LUGAR_SENTARSE);
}

void clienteLiberarLugarParaSentarse(Cliente_handler* handler) {
	//No hace nada en si, lo hace al desregistrarse
}

void clienteHacerPedido(Cliente_handler* handler, Mensaje_gustos* mensaje) {
	MessageQ msgq;
	msgq.mtype = 1;
	sprintf(msgq.type,"%d",MSG_BROKER_PEDIDO);
	serializeMsgGusto(mensaje,msgq.payload);

	enviarmsgq(handler->_msgq_id_enviar,&msgq,sizeof(MessageQ));
}

void clienteRecibirTicket(Cliente_handler* handler, Mensaje_ticket* mensaje) {
	MessageQ msgq;
	recibirmsgq(handler->_msgq_id_recibir_momId,&msgq,sizeof(MessageQ),handler->id);
	deserializeMsgTicket(mensaje,msgq.payload);
}

void clienteRecibirHelado(Cliente_handler* handler, Mensaje_helado* mensaje, int ticket) {
	MessageQ msgq;
	recibirmsgq(handler->_msgq_id_recibir_ticket,&msgq,sizeof(MessageQ),ticket);
	deserializeMsgHelado(mensaje,msgq.payload);
}

#endif //TPS_DISTRIBUIDOS_ICLIENTEMOM_H