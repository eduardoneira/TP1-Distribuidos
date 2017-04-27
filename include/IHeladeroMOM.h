#ifndef TPS_DISTRIBUIDOS_IHELADEROMOM_H
#define TPS_DISTRIBUIDOS_IHELADEROMOM_H

#include "heladero_handler.h"
#include "semaforo.h"
#include "mensaje_gustos.h"
#include "mensaje_helado.h"
#include "msg_queue.h"

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
    handler._msgq_id_CH = getmsgq(MSGQ_CAJERO_A_HELADEROS);
    handler._msgq_id_HC = getmsgq(MSGQ_HELADEROS_A_CLIENTES);
    return handler;
}

void ocuparHelado(Heladero_handler* handler, int gusto) {
    p(handler->_helados.at(gusto),0);
}

void liberarHelado(Heladero_handler* handler, int gusto) {
    v(handler->_helados.at(gusto),0);
}

void recibirPedidoDeCajero(Heladero_handler* handler, Mensaje_gustos* msg) {
    recibirmsgq(handler->_msgq_id_CH,msg,sizeof(Mensaje_gustos),MENSAJE_A_HELADERO);
}

void enviarAManegerQueMeVoy(Heladero_handler* handler) {
    Mensaje_gustos msg;
    crearMsgIrse(&msg);
    msg.mtype = MENSAJE_A_MANAGER;
    enviarmsgq(handler->_msgq_id_HC,&msg,sizeof(Mensaje_gustos));
}

void enviarPedidoACliente(Heladero_handler* handler, Mensaje_helado* msg) {
    enviarmsgq(handler->_msgq_id_HC,msg,sizeof(Mensaje_helado));
}

void cerrarHeladero(Heladero_handler* handler){
    handler->_helados.clear();
    handler->_msgq_id_CH = -1;
    handler->_msgq_id_HC = -1;
}



#endif //TPS_DISTRIBUIDOS_IHELADEROMOM_H
