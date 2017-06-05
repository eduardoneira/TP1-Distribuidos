
#ifndef TPS_DISTRIBUIDOS_BROKER_ROUTER_H
#define TPS_DISTRIBUIDOS_BROKER_ROUTER_H

#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/message_wrapper.h"
#include "../include/estado_heladeria.h"
#include <vector>

typedef struct Entry_router{
    int momId;
    int mtype;
} Entry_router;

typedef struct Router_handler {
    std::vector<Entry_router> heladeros;
    std::vector<Entry_router> cajeros;
    std::vector<Entry_router> clientes;
    int heladerosTurno;
    int cajerosTurno;
    EstadoHeladeria estado;
    std::vector<char[128]> helados;
} Router_handler;

Router_handler crearRouterHandler() {
    Router_handler handler;
    handler.heladerosTurno = 0;
    handler.cajerosTurno = 0;
    handler.estado.tamanio_cola = TAMANIO_COLA;
    handler.estado.tamanio_heladeria = TAMANIO_HELADERIA;
    handler.estado.abierto = CERRADO;
    return handler;
}

#endif //TPS_DISTRIBUIDOS_BROKER_ROUTER_H
