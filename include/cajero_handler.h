#ifndef TPS_DISTRIBUIDOS_CAJERO_HANDLER_H
#define TPS_DISTRIBUIDOS_CAJERO_HANDLER_H

typedef struct Cajero_handler {
    int _msgq_id_recibir_pedido;
    int _msgq_id_pasar_pedido_heladero;
    int _msgq_id_enviar_ticket;
    int id;
} Cajero_handler;

#endif //TPS_DISTRIBUIDOS_CAJERO_HANDLER_H
