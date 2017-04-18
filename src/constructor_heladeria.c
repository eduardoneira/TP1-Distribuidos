#include <stdio.h>
#include <unistd.h>

#include "../include/constructor.h"

int main (int argc, char ** argv){
	
	crearIPC();

	//Lanzo al cajero
	if (fork() == 0) {
		execl("./cajero","./cajero",(char*) NULL);
		perror("Exec fallo");
		return -1;
	}

	// Lanzo a los heladeros
	if (fork() == 0) {
		execl("./heladero","./heladero",(char*) NULL);
		perror("Exec fallo");
		return -1;
	}

	if (fork() == 0) {
		execl("./heladero","./heladero",(char*) NULL);
		perror("Exec fallo");
		return -1;
	}


	return 0;
} 	