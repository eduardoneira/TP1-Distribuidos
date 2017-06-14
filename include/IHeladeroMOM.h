#ifndef TPS_DISTRIBUIDOS_IHELADEROMOM_H
#define TPS_DISTRIBUIDOS_IHELADEROMOM_H

#include "heladero_handler.h"
#include "semaforo.h"
#include "mensaje_gustos.h"
#include "mensaje_helado.h"
#include "msg_queue.h"
#include "mensaje_registro.h"
#include "mensaje_semaforo.h"

Heladero_handler registrarHeladero() {
    Heladero_handler handler;
    handler._msgq_id_recibir_pedido = getmsgq(MSGQ_POR_MOMID_HELADERO_PEDIDO);
    handler._msgq_id_recibir_bocha = getmsgq(MSGQ_POR_MOMID_HELADERO_BOCHA);
    handler._msgq_id_enviar = getmsgq(MSGQ_RECIBIR_HELADERO);
    handler.id = registrarse(handler._msgq_id_enviar,HELADERO);
    return handler;
}

void enviarMensajeHelado(Heladero_handler* handler, int gusto,int tipo,bool respuesta){
    Mensaje_semaforo msg_sem;
    msg_sem.mtype = handler->id;
    msg_sem.index = gusto;

    MessageQ msg;
    msg.mtype = 1;
    sprintf(msg.type,"%d",tipo);
    serializeMsgSemaforo(&msg_sem,msg.payload);

    enviarmsgq(handler->_msgq_id_enviar,&msg,sizeof(MessageQ));
    if (respuesta){
        recibirmsgqSinCheckeo(handler->_msgq_id_recibir_bocha,&msg,sizeof(MessageQ),handler->id);
    }
}

void ocuparHelado(Heladero_handler* handler, int gusto) {
    enviarMensajeHelado(handler,gusto,MSG_BROKER_OCUPAR_HELADO,true);
}

void liberarHelado(Heladero_handler* handler, int gusto) {
    enviarMensajeHelado(handler,gusto,MSG_BROKER_DESOCUPAR_HELADO,false);
}

void recibirPedidoDeCajero(Heladero_handler* handler, Mensaje_gustos* msg) {
    MessageQ msgq;
    if (recibirmsgqSinCheckeo(handler->_msgq_id_recibir_pedido,&msgq,sizeof(MessageQ),handler->id) == -1){
        crearMsgIrse(msg);
    } else {
        deserializeMsgGusto(msg,msgq.payload);    
    }
    
}

void enviarAManagerQueMeVoy() {
    //Ahora no hace nada
}

void enviarPedidoACliente(Heladero_handler* handler, Mensaje_helado* msg) {
    MessageQ msgq;
    sprintf(msgq.type,"%d",MSG_BROKER_HELADO);
    msgq.mtype = 1;
    serializeMsgHelado(msg,msgq.payload);
    enviarmsgq(handler->_msgq_id_enviar,&msgq,sizeof(MessageQ));
}

void cerrarHeladero(Heladero_handler* handler){
    desregistrarse(handler->_msgq_id_enviar,handler->id,HELADERO);
}

#endif //TPS_DISTRIBUIDOS_IHELADEROMOM_H
