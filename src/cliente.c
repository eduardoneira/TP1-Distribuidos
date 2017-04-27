#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "../include/logger.h"
#include "../include/mensaje_gustos.h"
#include "../include/mensaje_ticket.h"
#include "../include/mensaje_helado.h"
#include "../include/IClienteMOM.h"


#define CLIENTE 				"CLIENTE\t"
#define TIEMPO_COMER_HELADO		5

bool hayLugarEnLaHeladeria(Logger* log, pid_t pid, EstadoHeladeria* estado,int semid, bool* paraLlevar) {
	bool meQuedo = true;

	p(semid,0);

	if (estado->abierto == ABIERTO) {
		if (estado->tamanio_cola > 0) {
			estado->tamanio_cola =  estado->tamanio_cola - 1;
			if (estado->tamanio_heladeria > 0) {
				estado->tamanio_heladeria = estado->tamanio_heladeria - 1;
				escribirLog(log,DEBUG,pid,CLIENTE,"Hay lugar, me voy a quedar a comer el helado");
				*paraLlevar = false;
			} else {
				escribirLog(log,DEBUG,pid,CLIENTE,"Voy a pedir el helado para llevar");
				*paraLlevar = true;
			}

		} else {
			escribirLog(log,DEBUG,pid,CLIENTE,"No hay lugar en la cola, me voy a casa");
			meQuedo = false;
		}
	} else {
		escribirLog(log,DEBUG,pid,CLIENTE,"La heladeria estaba cerrada, me voy a casa");
		meQuedo = false;
	}

	v(semid,0);

	return meQuedo;
}

int main(int argc, char** argv) {

	pid_t pid = getpid();
	Logger log = crearLogger();
	
	escribirLog(&log,DEBUG,pid,CLIENTE,"Hola soy un cliente, quiero helado");

	Cliente_handler handler = registrarCliente();

	if (clienteEntrarEnLaHeladeria(&handler)) {
		if (clienteEntrarEnLaCola(&handler)) {
			bool tengoAsiento = clienteReservarLugarParaSentarse(&handler);

			Mensaje_gustos msg_gustos;
			Mensaje_ticket msg_ticket;
			Mensaje_helado msg_helado;

			crearMsgGustos(&msg_gustos,MENSAJE_A_CAJERO,pid,generarNumeroRandomConSeed(CANTIDAD_GUSTOS,pid),generarNumeroRandom(CANTIDAD_GUSTOS),generarNumeroRandom(CANTIDAD_GUSTOS));
			char buffer[BUFFER_SIZE];

			//Hago el pedido
			sprintf(buffer,"Voy a pedir un helado de %d, %d y %d",msg_gustos.gustos_helado[0],msg_gustos.gustos_helado[1],msg_gustos.gustos_helado[2]);
			escribirLog(&log,DEBUG,pid,CLIENTE,buffer);
			clienteHacerPedido(&handler, &msg_gustos);

			//Espero el ticket
			clienteRecibirTicket(&handler, &msg_ticket, pid);
			sprintf(buffer,"Me dieron el ticket %d",msg_ticket.ticket);
			escribirLog(&log,DEBUG,pid,CLIENTE,buffer);

			//salgo de la cola
			clienteSalirDeLaCola(&handler);

			clienteRecibirHelado(&handler, &msg_helado, msg_ticket.ticket);
			sprintf(buffer,"Me dieron el helado %s",msg_helado.helado);
			escribirLog(&log,DEBUG,pid,CLIENTE,buffer);		

			if (tengoAsiento) {
				int random = generarNumeroRandom(TIEMPO_COMER_HELADO);
				sprintf(buffer,"Voy a comer mi helado por %d segundos",random);
				escribirLog(&log,DEBUG,pid,CLIENTE,buffer);
				sleep(random);
				escribirLog(&log,DEBUG,pid,CLIENTE,"Ya lo morfe, me voy");

				clienteLiberarLugarParaSentarse(&handler);
			} else {
				escribirLog(&log,DEBUG,pid,CLIENTE,"Listo, recibi mi helado gracias");
			}
		} else {
			escribirLog(&log,DEBUG,pid,CLIENTE,"No hay lugar en la cola, me voy a casa");
		}
	} else {
		escribirLog(&log,DEBUG,pid,CLIENTE,"La heladeria estaba cerrada, me voy a casa");
	}	

	desregistrarCliente(&handler);
	escribirLog(&log,DEBUG,pid,CLIENTE,"Chau");
	cerrarLogger(&log);

	return 0;
}