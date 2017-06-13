#ifndef TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H
#define TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H

#include "cajero_handler.h"
#include "mensaje_gustos.h"
#include "mensaje_ticket.h"
#include "mensaje_registro.h"
#include "msg_queue.h"
#include "rpc_wrapper.h"

Cajero_handler registrarCajero() {
    Cajero_handler handler;
    handler._msgq_id_enviar_ticket = getmsgq(MSGQ_PASAMANOS_CAJERO_MOM_TICKET);
    handler._msgq_id_pasar_pedido_heladero = getmsgq(MSGQ_PASAMANOS_CAJERO_MOM_PEDIDO);
    handler._msgq_id_recibir_pedido = getmsgq(MSGQ_PASAMANOS_MOM_CAJERO_PEDIDO);
    handler.id = registrarse();
    
    char host[20];
    getHostRPC(host,CAJERO);
    handler._rpc_ticket = initRPC(host);
    return handler;
}

int generarTicket(Cajero_handler* handler) {
    return getTicketRPC(handler->_rpc_ticket);
}

void recibirPedido(Cajero_handler* handler, Mensaje_gustos* msg) {
    recibirmsgq(handler->_msgq_id_recibir_pedido,msg,sizeof(Mensaje_gustos),MENSAJE_A_CAJERO);
}

void enviarTicketACliente(Cajero_handler* handler, Mensaje_ticket* msg) {
    enviarmsgq(handler->_msgq_id_enviar_ticket,msg,sizeof(Mensaje_ticket));
}

void enviarPedidoAHeladero(Cajero_handler* handler, Mensaje_gustos* msg, int ticket) {
    msg->id = ticket;
    enviarmsgq(handler->_msgq_id_pasar_pedido_heladero,msg,sizeof(Mensaje_gustos));
}

void enviarMsgQueMeVoy() {
    Mensaje_gustos msg;
    crearMsgIrse(&msg);
    enviarmsgq(getmsgq(MSGQ_DESTRUCTOR),&msg,sizeof(Mensaje_gustos));
}

void cerrarCajero(Cajero_handler* handler) {
    handler->_msgq_id_recibir_pedido = -1;
    handler->_msgq_id_enviar_ticket = -1;
    handler->_msgq_id_pasar_pedido_heladero = -1;
    destruirRPC(handler->_rpc_ticket);
}

#endif //TPS_DISTRIBUIDOS_ICAJEROCONCLIENTEMON_H
