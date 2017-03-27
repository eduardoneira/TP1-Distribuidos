#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

#include "../include/logger.h"
#include "../include/mensaje_gustos.h"
#include "../include/mensaje_ticket.h"
#include "../include/msg_queue.h"

#define HELADERO "HELADERO"

int main(int argc, char** argv) {
	
	Logger log = crearLogger();
	pid_t pid = getpid();

	escribirLog(&log,DEBUG,pid,HELADERO,"Hola soy un heladero, voy a esperar a que el cajero me gire pedidos");


	// Para saber si tengo que eliminar todo
	int meVoy = 0;

	return 0;
}