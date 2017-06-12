#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "../include/logger.h"
#include "../include/mensaje_gustos.h"
#include "../include/mensaje_helado.h"
#include "../include/IHeladeroMOM.h"

#define MIN_TIME 	2

int main(int argc, char** argv) {
	
	Logger log = crearLogger();
	pid_t pid = getpid();
	char buffer[BUFFER_SIZE];

	escribirLog(&log,DEBUG,pid,HELADERO_NAME,"Hola soy un heladero, voy a esperar a que el cajero me gire pedidos");
	Heladero_handler handler = registrarHeladero();
	Mensaje_gustos msg_gustos;
	Mensaje_helado msg_helado;

    sprintf(buffer,"Me identifican con %d",handler.id);
    escribirLog(&log,DEBUG,pid,HELADERO_NAME,buffer);


    // Para saber si tengo que eliminar todo
	bool meVoy = false;

	//init de random
	generarNumeroRandomConSeed(pid,pid);

	while (!meVoy) {

		recibirPedidoDeCajero(&handler,&msg_gustos);
		sprintf(buffer,"Recibi un mensaje del cajero con ticket %d",msg_gustos.id);
		escribirLog(&log,DEBUG,pid,HELADERO_NAME,buffer);

		if (esMsgDeIrse(&msg_gustos)) {
			enviarAManagerQueMeVoy();
			escribirLog(&log,DEBUG,pid,HELADERO_NAME,"Voy a irme, nos vemos");
			meVoy = true;
		} else {
			int i;
			char aux[BUFFER_SIZE];
			sprintf(buffer,"Helado de");
			
			for (i = 0; i < GUSTOS_POR_PERSONA; i++) {
				ocuparHelado(&handler,msg_gustos.gustos_helado[i]);
				sprintf(aux," %d",msg_gustos.gustos_helado[i]);
				strcat(buffer,aux);
				//Simulo que me lleva tiempo armar el helado, podria logearlo
				sleep(generarNumeroRandomConMin(MIN_TIME,MIN_TIME));
				liberarHelado(&handler,msg_gustos.gustos_helado[i]);
			}

			strcpy(aux,buffer);

			crearMensajeHelado(&msg_helado,msg_gustos.id,aux);
			sprintf(buffer,"Voy a enviar un %s",aux);
			escribirLog(&log,DEBUG,pid,HELADERO_NAME,buffer);
			
			enviarPedidoACliente(&handler,&msg_helado);

		}
	}

	cerrarHeladero(&handler);	
	cerrarLogger(&log);	

	return 0;
}