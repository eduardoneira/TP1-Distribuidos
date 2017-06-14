#ifndef TPS_DISTRIBUIDOS_HELADERO_HANDLER_H
#define TPS_DISTRIBUIDOS_HELADERO_HANDLER_H

typedef struct Heladero_handler {
    int _msgq_id_recibir_pedido;
    int _msgq_id_recibir_bocha;
    int _msgq_id_enviar;
    int id;
} Heladero_handler;

#endif //TPS_DISTRIBUIDOS_HELADERO_HANDLER_H
