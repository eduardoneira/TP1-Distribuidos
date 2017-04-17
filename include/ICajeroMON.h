#ifndef TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H
#define TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H

#include "cajero_handler.h"
#include "mensaje_gustos.h"
#include "mensaje_ticket.h"
#include "msg_queue.h"

#define MIN_TIME	3

Cajero_handler registrarCajero() {
    Cajero_handler handler;
    handler._msgq_id_CC = getmsgq(MSGQ_CLIENTES_AL_CAJERO);
    handler._msgq_id_CH = getmsgq(MSGQ_CAJERO_A_HELADEROS);
    return handler;
}

void recibirPedido(Cajero_handler* handler, Mensaje_gustos* msg) {
    recibirmsgq(handler->_msgq_id_CC,msg,sizeof(Mensaje_gustos),MENSAJE_A_CAJERO);
}

void enviarTicketACliente(Cajero_handler* handler, Mensaje_ticket* msg) {
    sleep(generarNumeroRandomConMin(MIN_TIME,MIN_TIME));
    enviarmsgq(handler->_msgq_id_CC,msg,sizeof(Mensaje_ticket));
}

void enviarPedidoAHeladero(Cajero_handler* handler, Mensaje_gustos* msg, int ticket) {
    msg->id = ticket;
    enviarmsgq(handler->_msgq_id_CH,msg,sizeof(Mensaje_gustos));
}

void enviarMsgQueMeVoy(Cajero_handler* handler) {
    Mensaje_gustos msg;
    msg.mtype = MENSAJE_A_MANAGER;
    enviarmsgq(handler->_msgq_id_CC,&msg,sizeof(Mensaje_gustos));
}

#endif //TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H
