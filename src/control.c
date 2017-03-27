#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>

#include "../include/logger.h"
#include "../include/semaforo.h"
#include "../include/memoria_compartida.h"
#include "../include/estado_heladeria.h"

#define MANAGER "MANAGER\t"

int main(int argc, char** argv) {
	pid_t pid = getpid();

	Logger log = crearLogger();

	if (argc == 1) {
		int shmid = getshm(ID_SHM_ESTADO_HELADERIA);
		int semid = getsem(SEMID_ESTADO_HELADERIA,1);

		p(semid,0);
		EstadoHeladeria* estado = (EstadoHeladeria*) map(shmid);
		if(estado->abierto == CERRADO){
			estado->abierto = ABIERTO;
			escribirLog(&log,DEBUG,pid,MANAGER,"Se vuelve a abrir la heladeria");
		} else {
			estado->abierto = CERRADO;
			escribirLog(&log,DEBUG,pid,MANAGER,"Se cierra la heladeria");
		}
		unmap(estado);
		v(semid,0);
		
	} else {
		char buffer[BUFFER_SIZE];
		
		sprintf(buffer,"Voy a lanzar %s clientes",argv[1]);
		escribirLog(&log,DEBUG,pid,MANAGER,buffer);

		int clientes = atoi(argv[1]);
		int i;

		for (i = 0; i < clientes; i++){
			if (fork() == 0) {
				execl("./cliente","./cliente",(char*) NULL);
				perror("Exec fallo");
				return -1;
			}
		}
	}

	cerrarLogger(&log);
}

