#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../include/memoria_compartida.h"
#include "../include/semaforo.h"
#include "../include/msg_queue.h"
#include "../include/lockfile.h"
#include "../include/fifo_lectura.h"
#include "../include/fifo_escritura.h"

int main (int argc, char ** argv){
	
	char msg[20];

	strcpy(msg,"Hello there\n");

	int fd;

	fd = abrirLock("log.txt");

	tomarLock(fd);

	escribir(fd,(void*) msg, strlen(msg)*sizeof(char));

	liberarLock(fd);

	cerrarLock(fd);

	//eliminarLock("log.txt");

	return 0;
} 	