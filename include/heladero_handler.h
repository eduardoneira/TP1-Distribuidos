#ifndef TPS_DISTRIBUIDOS_HELADERO_HANDLER_H
#define TPS_DISTRIBUIDOS_HELADERO_HANDLER_H

#include <vector>

typedef struct Heladero_handler {
    std::vector<int> _helados;
    int _msgq_id_recibir_pedido;
    int _msgq_id_enviar_pedido;
    int id;
} Heladero_handler;

#endif //TPS_DISTRIBUIDOS_HELADERO_HANDLER_H
