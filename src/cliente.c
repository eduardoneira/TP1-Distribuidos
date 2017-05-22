#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../include/logger.h"
#include "../include/mensaje_gustos.h"
#include "../include/mensaje_ticket.h"
#include "../include/mensaje_helado.h"
#include "../include/IClienteMOM.h"

#define TIEMPO_COMER_HELADO		5

int main(int argc, char** argv) {

	pid_t pid = getpid();
	Logger log = crearLogger();
	
	escribirLog(&log,DEBUG,pid,CLIENTE_NAME,"Hola soy un cliente, quiero helado");

	Cliente_handler handler = registrarCliente();

	if (clienteEntrarEnLaHeladeria(&handler)) {
		if (clienteEntrarEnLaCola(&handler)) {
			bool tengoAsiento = clienteReservarLugarParaSentarse(&handler);

			Mensaje_gustos msg_gustos;
			Mensaje_ticket msg_ticket;
			Mensaje_helado msg_helado;

			crearMsgGustos(&msg_gustos,MENSAJE_A_CAJERO,handler.id,generarNumeroRandomConSeed(CANTIDAD_GUSTOS,pid),generarNumeroRandom(CANTIDAD_GUSTOS),generarNumeroRandom(CANTIDAD_GUSTOS));
			char buffer[BUFFER_SIZE];

			//Hago el pedido
			sprintf(buffer,"Voy a pedir un helado de %d, %d y %d",msg_gustos.gustos_helado[0],msg_gustos.gustos_helado[1],msg_gustos.gustos_helado[2]);
			escribirLog(&log,DEBUG,pid,CLIENTE_NAME,buffer);
			clienteHacerPedido(&handler, &msg_gustos);

			//Espero el ticket
			clienteRecibirTicket(&handler, &msg_ticket);
			sprintf(buffer,"Me dieron el ticket %d",msg_ticket.ticket);
			escribirLog(&log,DEBUG,pid,CLIENTE_NAME,buffer);

			//salgo de la cola
			clienteSalirDeLaCola(&handler);

			clienteRecibirHelado(&handler, &msg_helado, msg_ticket.ticket);
			sprintf(buffer,"Me dieron el helado %s",msg_helado.helado);
			escribirLog(&log,DEBUG,pid,CLIENTE_NAME,buffer);

			if (tengoAsiento) {
				int random = generarNumeroRandom(TIEMPO_COMER_HELADO);
				sprintf(buffer,"Voy a comer mi helado por %d segundos",random);
				escribirLog(&log,DEBUG,pid,CLIENTE_NAME,buffer);
				sleep(random);
				escribirLog(&log,DEBUG,pid,CLIENTE_NAME,"Ya lo morfe, me voy");

				clienteLiberarLugarParaSentarse(&handler);
			} else {
				escribirLog(&log,DEBUG,pid,CLIENTE_NAME,"Listo, recibi mi helado gracias");
			}
		} else {
			escribirLog(&log,DEBUG,pid,CLIENTE_NAME,"No hay lugar en la cola, me voy a casa");
		}
	} else {
		escribirLog(&log,DEBUG,pid,CLIENTE_NAME,"La heladeria estaba cerrada, me voy a casa");
	}	

	desregistrarCliente(&handler);
	escribirLog(&log,DEBUG,pid,CLIENTE_NAME,"Chau");
	cerrarLogger(&log);

	return 0;
}