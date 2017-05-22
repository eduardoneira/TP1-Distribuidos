#ifndef TPS_DISTRIBUIDOS_CLIENTE_HANDLER_H
#define TPS_DISTRIBUIDOS_CLIENTE_HANDLER_H

#include "estado_heladeria.h"

typedef struct Cliente_handler {
    EstadoHeladeria* _estadoHeladeria;
    int _semidEstadoHeladeria;

	int _msgq_id_enviar_pedido;
	int _msgq_id_recibir_ticket;
	int _msgq_id_recibir_helado;
	int id;
} Cliente_handler;

#endif //TPS_DISTRIBUIDOS_CLIENTE_HANDLER_H