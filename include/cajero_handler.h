#ifndef TPS_DISTRIBUIDOS_CAJERO_HANDLER_H
#define TPS_DISTRIBUIDOS_CAJERO_HANDLER_H

#include "../rpc/rpc.h"

typedef struct Cajero_handler {
    int _msgq_id_recibir;
    int _msgq_id_enviar;
    int id;
} Cajero_handler;

#endif //TPS_DISTRIBUIDOS_CAJERO_HANDLER_H
