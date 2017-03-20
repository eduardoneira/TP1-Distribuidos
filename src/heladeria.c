#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#include "../include/msg_queue.h"
#include "../include/lockfile.h"
#include "../include/fifo_lectura.h"
#include "../include/fifo_escritura.h"
#include "../include/estado_heladeria.h"

int checkargc(int argc){
	if (argc != 3) {
		printf("Se necesitan 2 argumentos para correr, llamar al programa con ./heladeria N M");
		exit(-1);
	}
}

int main (int argc, char ** argv){
	
	EstadoHeladeria estadoHeladeria = crearEstadoHeladeria();

	

	for (int i = 0; i < 10; i++){
		if (fork() == 0){
			execl("./cliente","./cliente",(char*) NULL);
			perror("exec fallo");
			return -1;
		}		
	}


	sleep(generarNumeroRandom());

	eliminarEstadoHeladeria(estadoHeladeria);

	return 0;
} 	