#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <vector>

#include "../include/logger.h"
#include "../include/semaforo.h"
#include "../include/mensaje_gustos.h"
#include "../include/mensaje_helado.h"
#include "../include/msg_queue.h"

#define HELADERO 	"HELADERO\t"
#define MIN_TIME 	2

std::vector<int> crearAccesoGustosHelado() {
	std::vector<int> semids;
	int i;
	
	for (i = PRIMER_ID_GUSTO_HELADO; i < PRIMER_ID_GUSTO_HELADO + CANTIDAD_GUSTOS;i++) {
		semids.push_back(getsem(i,1));
	}

	return semids;
}

int main(int argc, char** argv) {
	
	Logger log = crearLogger();
	pid_t pid = getpid();
	char buffer[BUFFER_SIZE];

	escribirLog(&log,DEBUG,pid,HELADERO,"Hola soy un heladero, voy a esperar a que el cajero me gire pedidos");

	//IPC COMUNICACION
	int msgq_id_CH = getmsgq(MSGQ_CAJERO_A_HELADEROS);
	int msgq_id_HC = getmsgq(MSGQ_HELADEROS_A_CLIENTES);
	Mensaje_gustos msg_gustos;
	Mensaje_helado msg_helado;

	std::vector<int> semids = crearAccesoGustosHelado();

	// Para saber si tengo que eliminar todo
	bool meVoy = false;

	//init de random
	generarNumeroRandomConSeed(pid,pid);

	while (!meVoy) {

		recibirmsgq(msgq_id_CH,&msg_gustos,sizeof(Mensaje_gustos),MENSAJE_A_HELADERO);
		sprintf(buffer,"Recibi un mensaje del cajero con ticket %d",msg_gustos.id);
		escribirLog(&log,DEBUG,pid,HELADERO,buffer);

		if (esMsgDeIrse(&msg_gustos)) {
			msg_gustos.mtype = MENSAJE_A_MANAGER;
			enviarmsgq(msgq_id_CH,&msg_gustos,sizeof(Mensaje_gustos));
			escribirLog(&log,DEBUG,pid,HELADERO,"Voy a irme, nos vemos");
			meVoy = true;
		} else {
			int i;
			char aux[BUFFER_SIZE];
			sprintf(buffer,"Helado de");
			
			for (i = 0; i < GUSTOS_POR_PERSONA; i++) {
				p(semids.at(msg_gustos.gustos_helado[i]),0);
				sprintf(aux," %d",msg_gustos.gustos_helado[i]);
				strcat(buffer,aux);
				//Simulo que me lleva tiempo armar el helado, podria logearlo
				sleep(generarNumeroRandomConMin(MIN_TIME,MIN_TIME));
				v(semids.at(msg_gustos.gustos_helado[i]),0);
			}

			strcpy(aux,buffer);

			crearMensajeHelado(&msg_helado,msg_gustos.id,aux);
			sprintf(buffer,"Voy a enviar un %s",aux);
			escribirLog(&log,DEBUG,pid,HELADERO,buffer);
			
			enviarmsgq(msgq_id_HC,&msg_helado,sizeof(Mensaje_helado));

		}
	}




	cerrarLogger(&log);	

	return 0;
}