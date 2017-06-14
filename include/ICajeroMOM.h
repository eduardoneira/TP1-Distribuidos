#ifndef TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H
#define TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H

#include "cajero_handler.h"
#include "mensaje_gustos.h"
#include "mensaje_ticket.h"
#include "mensaje_registro.h"
#include "msg_queue.h"

Cajero_handler registrarCajero() {
    Cajero_handler handler;
    handler._msgq_id_enviar = getmsgq(MSGQ_RECIBIR_CAJERO);
    handler._msgq_id_recibir_ticket = getmsgq(MSGQ_POR_MOMID_CAJERO_TICKET);
    handler._msgq_id_recibir_pedido = getmsgq(MSGQ_POR_MOMID_CAJERO_PEDIDO);
    handler.id = registrarse(handler._msgq_id_enviar,CAJERO);
    return handler;
}

int generarTicket(Cajero_handler* handler) {
    MessageQ msgq;
    sprintf(msgq.type,"%d",MSG_BROKER_GENERAR_TICKET);
    msgq.mtype = 1;

    Mensaje_ticket msg;
    msg.mtype = handler->id;
    msg.ticket = -1;
    serializeMsgTicket(&msg,msgq.payload);

    enviarmsgq(handler->_msgq_id_enviar,&msgq,sizeof(MessageQ));
    recibirmsgqSinCheckeo(handler->_msgq_id_recibir_ticket,&msgq,sizeof(MessageQ),handler->id);

    deserializeMsgTicket(&msg,msgq.payload);

    return msg.ticket;
}

void recibirPedido(Cajero_handler* handler, Mensaje_gustos* msg_gustos) {
    MessageQ msg;
    if (recibirmsgqSinCheckeo(handler->_msgq_id_recibir_pedido,&msg,sizeof(MessageQ),handler->id) == -1) {
        crearMsgIrse(msg_gustos);
    } else{
        deserializeMsgGusto(msg_gustos,msg.payload);    
    } 
    
}

void enviarTicketACliente(Cajero_handler* handler, Mensaje_ticket* msg) {
    MessageQ msgq;
    sprintf(msgq.type,"%d",MSG_BROKER_TICKET);
    msgq.mtype = 1;
    serializeMsgTicket(msg,msgq.payload);

    enviarmsgq(handler->_msgq_id_enviar,&msgq,sizeof(MessageQ));
}

void enviarPedidoAHeladero(Cajero_handler* handler, Mensaje_gustos* msg, int ticket) {
    msg->id = ticket;
    msg->momId = MENSAJE_A_HELADERO;
    MessageQ msgq;
    sprintf(msgq.type,"%d",MSG_BROKER_PEDIDO);
    msgq.mtype = 1;
    serializeMsgGusto(msg,msgq.payload);

    enviarmsgq(handler->_msgq_id_enviar,&msgq,sizeof(MessageQ));
}

void enviarMsgQueMeVoy() {
    //Ahora no hago nada 
}

void cerrarCajero(Cajero_handler* handler) {
    desregistrarse(handler->_msgq_id_enviar,handler->id,CAJERO);
}

#endif //TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H
