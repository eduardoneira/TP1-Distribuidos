#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

#include "../include/logger.h"
#include "../include/mensaje_gustos.h"
#include "../include/mensaje_ticket.h"
#include "../include/msg_queue.h"

#define CAJERO "CAJERO\t\t" 

int main(int argc, char** argv) {
	
	Logger log = crearLogger();
	pid_t pid = getpid();
	char buffer[BUFFER_SIZE];
		
	escribirLog(&log,DEBUG,pid,CAJERO,"Hola soy el cajero, voy a empezar a escuchar clientes");

	int ticket = 1;

	// Para saber si tengo que eliminar todo
	bool meVoy = false;

	//Todo lo que necesito de ipc
	int msgq_id_CC = getmsgq(MSGQ_CLIENTES_AL_CAJERO);
	int msgq_id_CH = getmsgq(MSGQ_CAJERO_A_HELADEROS);
	Mensaje_gustos msg_gustos;
	Mensaje_ticket msg_ticket;

	while (!meVoy) {

		recibirmsgq(msgq_id_CC,&msg_gustos,sizeof(Mensaje_gustos),MENSAJE_A_CAJERO);
		sprintf(buffer,"Recibi un mensaje de %d :O",msg_gustos.id);
		escribirLog(&log,DEBUG,pid,CAJERO,buffer);

		if (esMsgDeIrse(&msg_gustos)) {
			msg_gustos.mtype = MENSAJE_A_MANAGER;
			enviarmsgq(msgq_id_CC,&msg_gustos,sizeof(Mensaje_gustos));
			escribirLog(&log,DEBUG,pid,CAJERO,"Voy a irme, nos vemos");
			meVoy = true;
		} else {
			crearMsgTicket(&msg_ticket,msg_gustos.id,ticket);

			escribirLog(&log,DEBUG,pid,CAJERO,"Le voy a pasar al cliente su ticket");
			enviarmsgq(msgq_id_CC,&msg_ticket,sizeof(Mensaje_ticket));

			msg_gustos.id = ticket;
			escribirLog(&log,DEBUG,pid,CAJERO,"Le paso a los heladeros el pedido");
			enviarmsgq(msgq_id_CH,&msg_gustos,sizeof(Mensaje_gustos));

			ticket = (ticket + 1) % INT_MAX;
		}
	}

	cerrarLogger(&log);

	return 0;
}