#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/constructor.h"

void execMOM(char* modo) {

	if (strcmp(modo,ALL) == 0 || strcmp(modo,CAJERO) == 0) {
		//Lanzo al MOM CAJERO
		if (fork() == 0) {
			execl("./MOM_server","./MOM_server",CAJERO,(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}

	}

    if (strcmp(modo,ALL) == 0 || strcmp(modo,HELADERO) == 0){
		//Lanzo al MOM HELADERO
		if (fork() == 0) {
			execl("./MOM_server","./MOM_server",HELADERO,(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}
	}

    if (strcmp(modo,ALL) == 0 || strcmp(modo,CLIENTE) == 0) {
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
		printf("Se debe especificar que crear, se pueden usar:\n heladero\n cajero\n cliente\n all\n broker\n");
		return 1;
	}

	crearIPC(argv[1]);

	execMOM(argv[1]);
/*
	if (strcmp(argv[1],ALL) == 0 || strcmp(argv[1],CAJERO) == 0) {
		//Lanzo a un cajero
		if (fork() == 0) {
			execl("./cajero", "./cajero", (char *) NULL);
			perror("Exec fallo");
			return -1;
		}
	}

	if (strcmp(argv[1],ALL) == 0 || strcmp(argv[1],HELADERO) == 0) {
		// Lanzo a un heladero
		if (fork() == 0) {
			execl("./heladero", "./heladero", (char *) NULL);
			perror("Exec fallo");
			return -1;
		}
	}

	if (strcmp(argv[1],BROKER) == 0) {
		//Lanzo al broker
		if (fork() == 0) {
			execl("./broker", "./broker", (char *) NULL);
			perror("Exec fallo");
			return -1;
		}
	}

*/
	return 0;
} 	