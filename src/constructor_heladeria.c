#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/constructor.h"

void execMOM(char* modo) {

	if (strcmp(modo,CAJERO) == 0) {
		//Lanzo al MOM CAJERO
		if (fork() == 0) {
			execl("./MOM_server","./MOM_server",CAJERO,(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}

	}

    if (strcmp(modo,HELADERO) == 0){
		//Lanzo al MOM HELADERO
		if (fork() == 0) {
			execl("./MOM_server","./MOM_server",HELADERO,(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}
	}

    if (strcmp(modo,CLIENTE) == 0) {
		//Lanzo al MOM CLIENTE
		if (fork() == 0) {
			execl("./MOM_server","./MOM_server",CLIENTE,(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}
	}

}

int main (int argc, char ** argv){

	if (argc == 1) {
		printf("Se debe especificar que crear, se pueden usar:\n heladero\n cajero\n cliente\n broker\n");
		return 1;
	}

	crearIPC(argv[1]);

	execMOM(argv[1]);

	return 0;
} 	