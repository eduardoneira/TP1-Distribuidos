#ifndef TPS_DISTRIBUIDOS_IHELADEROMOM_H
#define TPS_DISTRIBUIDOS_IHELADEROMOM_H

#include "heladero_handler.h"
#include "semaforo.h"
#include "mensaje_gustos.h"
#include "mensaje_helado.h"
#include "msg_queue.h"
#include "mensaje_registro.h"

std::vector<int> _crearAccesoGustosHelado() {
    std::vector<int> semids;

    for (int i = PRIMER_ID_GUSTO_HELADO; i < PRIMER_ID_GUSTO_HELADO + CANTIDAD_GUSTOS;i++) {
        semids.push_back(getsem(i,1));
    }

    return semids;
}

Heladero_handler registrarHeladero() {
    Heladero_handler handler;
    handler._helados = _crearAccesoGustosHelado();
    handler._msgq_id_recibir_pedido = getmsgq(MSGQ_PASAMANOS_MOM_HELADERO_PEDIDO);
    handler._msgq_id_enviar_pedido = getmsgq(MSGQ_PASAMANOS_HELADERO_MOM_HELADO);
    handler.id = registrarse();
    return handler;
}

void ocuparHelado(Heladero_handler* handler, int gusto) {
    p(handler->_helados.at(gusto),0);
}

void liberarHelado(Heladero_handler* handler, int gusto) {
    v(handler->_helados.at(gusto),0);
}

void recibirPedidoDeCajero(Heladero_handler* handler, Mensaje_gustos* msg) {
    recibirmsgq(handler->_msgq_id_recibir_pedido,msg,sizeof(Mensaje_gustos),MENSAJE_A_HELADERO);
}

void enviarAManagerQueMeVoy() {
    Mensaje_gustos msg;
    crearMsgIrse(&msg);
    enviarmsgq(getmsgq(MSGQ_DESTRUCTOR),&msg,sizeof(Mensaje_gustos));
}

void enviarPedidoACliente(Heladero_handler* handler, Mensaje_helado* msg) {
    enviarmsgq(handler->_msgq_id_enviar_pedido,msg,sizeof(Mensaje_helado));
}

void cerrarHeladero(Heladero_handler* handler){
    handler->_helados.clear();
    handler->_msgq_id_recibir_pedido = -1;
    handler->_msgq_id_enviar_pedido = -1;
}

#endif //TPS_DISTRIBUIDOS_IHELADEROMOM_H
