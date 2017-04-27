#ifndef TPS_DISTRIBUIDOS_ICLIENTEMOM_H
#define TPS_DISTRIBUIDOS_ICLIENTEMOM_H

#include "cliente_handler.h"
#include "msg_queue.h"
#include "memoria_compartida.h"
#include "semaforo.h"
#include "mensaje_gustos.h"
#include "mensaje_ticket.h"
#include "mensaje_helado.h"
#include "estado_heladeria.h"

Cliente_handler registrarCliente() {
	
	Cliente_handler handler;

	handler._estadoHeladeria = (EstadoHeladeria*) map(getshm(ID_SHM_ESTADO_HELADERIA));
	handler._semidEstadoHeladeria = getsem(SEMID_ESTADO_HELADERIA,1);
	
	handler._msgq_id_CC = -1;
	handler._msgq_id_HC = -1;

	return handler;
}

void desregistrarCliente(Cliente_handler* handler) {

	unmap((void*) handler->_estadoHeladeria);
}

bool clienteEntrarEnLaHeladeria(Cliente_handler* handler) {
	
	p(handler->_semidEstadoHeladeria,0);
	bool abierta = handler->_estadoHeladeria->abierto == ABIERTO;
	v(handler->_semidEstadoHeladeria,0);

	if (abierta) {
		handler->_msgq_id_CC = getmsgq(MSGQ_CLIENTES_AL_CAJERO);
		handler->_msgq_id_HC = getmsgq(MSGQ_HELADEROS_A_CLIENTES);
	}

	return abierta;
}

bool clienteEntrarEnLaCola(Cliente_handler* handler) {

	p(handler->_semidEstadoHeladeria,0);
	bool hayLugar = handler->_estadoHeladeria->tamanio_cola > 0;
	
	if (hayLugar) {
		handler->_estadoHeladeria->tamanio_cola =  handler->_estadoHeladeria->tamanio_cola - 1;
	}
	
	v(handler->_semidEstadoHeladeria,0);
	
	return hayLugar;
}

void clienteSalirDeLaCola(Cliente_handler* handler) {

	p(handler->_semidEstadoHeladeria,0);
	handler->_estadoHeladeria->tamanio_cola =  handler->_estadoHeladeria->tamanio_cola + 1;
	v(handler->_semidEstadoHeladeria,0);
}

bool clienteReservarLugarParaSentarse(Cliente_handler* handler) {

	p(handler->_semidEstadoHeladeria,0);
	bool hayLugarParaSentarse = handler->_estadoHeladeria->tamanio_heladeria > 0;
	
	if (hayLugarParaSentarse) {
		handler->_estadoHeladeria->tamanio_heladeria = handler->_estadoHeladeria->tamanio_heladeria - 1;
	}
	
	v(handler->_semidEstadoHeladeria,0);
	
	return hayLugarParaSentarse;
}

void clienteLiberarLugarParaSentarse(Cliente_handler* handler) {
	p(handler->_semidEstadoHeladeria,0);
	handler->_estadoHeladeria->tamanio_heladeria = handler->_estadoHeladeria->tamanio_heladeria + 1;
	v(handler->_semidEstadoHeladeria,0);
}

void clienteHacerPedido(Cliente_handler* handler, Mensaje_gustos* mensaje) {
	enviarmsgq(handler->_msgq_id_CC,mensaje,sizeof(Mensaje_gustos));
}

void clienteRecibirTicket(Cliente_handler* handler, Mensaje_ticket* mensaje, int nroCliente) {
	recibirmsgq(handler->_msgq_id_CC,mensaje,sizeof(Mensaje_ticket),nroCliente);
}

void clienteRecibirHelado(Cliente_handler* handler, Mensaje_helado* mensaje, int ticket) {
	recibirmsgq(handler->_msgq_id_HC,mensaje,sizeof(Mensaje_helado),ticket);
}

#endif //TPS_DISTRIBUIDOS_ICLIENTEMOM_H