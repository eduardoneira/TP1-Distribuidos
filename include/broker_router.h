
#ifndef TPS_DISTRIBUIDOS_BROKER_ROUTER_H
#define TPS_DISTRIBUIDOS_BROKER_ROUTER_H

#include "logger.h"
#include "msg_queue.h"
#include "message_wrapper.h"
#include "estado_heladeria.h"
#include "mensaje_registro.h"
#include "mensaje_ticket.h"
#include "mensaje_helado.h"
#include "mensaje_gustos.h"
#include "mensaje_semaforo.h"
#include <vector>

typedef struct Entry_router{
    int momId;
    long mtype;
} Entry_router;

typedef struct Router_handler {
    std::vector<Entry_router> heladeros;
    std::vector<Entry_router> cajeros;
    std::vector<Entry_router> clientes;
    int heladerosTurno;
    int cajerosTurno;
    int momId;
    int ticket;
    EstadoHeladeria estado;
    std::vector<char[128]> helados;
} Router_handler;

Router_handler crearRouterHandler() {
    Router_handler handler;
    handler.heladerosTurno = 0;
    handler.cajerosTurno = 0;
    handler.momId = 1;
    handler.ticket = 1;
    handler.estado.tamanio_cola = TAMANIO_COLA;
    handler.estado.tamanio_heladeria = TAMANIO_HELADERIA;
    handler.estado.abierto = CERRADO;
    return handler;
}

long _routeClient(Router_handler* handler, long momId){
    for (std::vector<Entry_router>::iterator it = handler->clientes.begin(); it != handler->clientes.end(); it++) {
        if ((*it).momId == (int)momId) {
            return (*it).mtype;
        }
    }
}

long _routeHeladero(Router_handler* handler, long momId){
    for (std::vector<Entry_router>::iterator it = handler->heladeros.begin(); it != handler->heladeros.end(); it++) {
        if ((*it).momId == (int)momId) {
            return (*it).mtype;
        }
    }
}

bool registrarMOM(Router_handler* handler, MessageQ* msgq){
    Mensaje_registro msg_reg;
    deserializeMsgRegistro(&msg_reg,msgq->payload);
    msg_reg.mtype = msg_reg.id;
    msg_reg.id = handler->momId;
    handler->momId++;

    Entry_router entry;
    entry.mtype = msgq->mtype;
    entry.momId = msg_reg.id;

    if (strcmp(msg_reg.tipo,HELADERO) == 0) {
        handler->heladeros.push_back(entry);
    } else if (strcmp(msg_reg.tipo,CAJERO) == 0) {
        handler->cajeros.push_back(entry);
    } else if (strcmp(msg_reg.tipo,HELADERO) == 0){
        handler->clientes.push_back(entry);
    }

    serializeMsgRegistro(&msg_reg,msgq->payload);
    return true;
}

bool enviarTicket(Router_handler* handler, MessageQ* msg){
    Mensaje_ticket msg_ticket;
    deserializeMsgTicket(&msg_ticket,msg->payload);


    msg->mtype = _routeClient(handler,msg_ticket.mtype);

    msg_ticket.ticket = handler->ticket;
    handler->ticket++;
    serializeMsgTicket(&msg_ticket,msg->payload);

    return true;
}

bool enviarHelado(Router_handler* handler, MessageQ* msg){
    Mensaje_helado msg_helado;
    deserializeMsgHelado(&msg_helado,msg->payload);

    msg->mtype = _routeClient(handler,msg_helado.momId);

    return true;
}

bool enviarPedido(Router_handler* handler, MessageQ* msg) {
    Mensaje_gustos msg_gustos;
    deserializeMsgGusto(&msg_gustos,msg->payload);

    if (msg_gustos.momId == MENSAJE_A_HELADERO) {
        msg->mtype = handler->heladeros.at(handler->heladerosTurno).mtype;
        handler->heladerosTurno = (handler->heladerosTurno + 1) % handler->heladeros.size();
    } else {
        msg->mtype = handler->cajeros.at(handler->cajerosTurno).mtype;
        handler->cajerosTurno = (handler->cajerosTurno + 1) % handler->cajeros.size();
    }

    return true;
}

bool ocuparHelado(Router_handler* handler, MessageQ* msg){
    Mensaje_semaforo msg_sem;
    deserializeMsgSemaforo(&msg_sem,msg->payload);

    char buffer[10];

    if (strlen(handler->helados.at(msg_sem.index)) > 0) {
        sprintf(buffer,"-%ld",msg_sem.mtype);
        strcat(handler->helados.at(msg_sem.index),buffer);
        return false;
    } else {
        sprintf(buffer,"%ld",msg_sem.mtype);
        strcat(handler->helados.at(msg_sem.index),buffer);
        return true;
    }

}

bool desocuparHelado(Router_handler* handler, MessageQ* msg) {
    Mensaje_semaforo msg_sem;
    deserializeMsgSemaforo(&msg_sem,msg->payload);

    char aux[128];
    strcpy(aux,handler->helados.at(msg_sem.index));

    strtok(aux,SEPARATOR);
    strcpy(handler->helados.at(msg_sem.index),strtok(NULL," "));
    if (strlen(handler->helados.at(msg_sem.index)) > 0) {
        strcpy(aux,handler->helados.at(msg_sem.index));
        long momId = atol(strtok(aux,SEPARATOR));

        msg_sem.mtype = momId;
        msg->mtype = _routeHeladero(handler,momId);
        sprintf(msg->type,"%d",MSG_BROKER_OCUPAR_HELADO);
        serializeMsgSemaforo(&msg_sem,msg->payload);
        return true;
    } else {
        return false;
    }

}

bool desregistrarse(Router_handler* handler, MessageQ* msg) {
    Mensaje_registro msg_reg;
    deserializeMsgRegistro(&msg_reg,msg->payload);
    int momId = msg_reg.id;
    int index = 0;

    if (strcmp(msg_reg.tipo,HELADERO) == 0) {
        for (std::vector<Entry_router>::iterator it = handler->heladeros.begin(); it != handler->heladeros.end(); it++) {
            if ((*it).momId == momId) {
                 handler->heladeros.erase(it);
            }
            index++;
        }

        if (index < handler->heladerosTurno) {
            handler->heladerosTurno--;
        }
    } else if (strcmp(msg_reg.tipo,CAJERO) == 0) {
        for (std::vector<Entry_router>::iterator it = handler->cajeros.begin(); it != handler->cajeros.end(); it++) {
            if ((*it).momId == momId) {
                handler->cajeros.erase(it);
            }
            index++;
        }

        if (index < handler->cajerosTurno) {
            handler->cajerosTurno--;
        }
    } else if (strcmp(msg_reg.tipo,CLIENTE) == 0){
        for (std::vector<Entry_router>::iterator it = handler->clientes.begin(); it != handler->clientes.end(); it++) {
            if ((*it).momId == momId) {
                handler->clientes.erase(it);
            }
        }
        handler->estado.tamanio_heladeria--;
    }

    return false;
}

bool cambiarEstadoHeladeria(Router_handler* handler, int estado){
    handler->estado.abierto = estado;
    return false;
}

void destruirRouterHandler(Router_handler* handler) {
}

#endif //TPS_DISTRIBUIDOS_BROKER_ROUTER_H
