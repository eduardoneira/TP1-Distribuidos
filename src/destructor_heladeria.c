#include <stdio.h>

#include "../include/destructor.h"

int main(int argc, char** argv) {

	if (argc == 1) {
		printf("Se debe especificar que crear, se pueden usar:\n heladero\n cajero\n cliente\n all\n");
		return 1;
	}

	//TODO : cambiar para la siguiente entrega
	argv[1] = ALL;

	return destruirIPC(argv[1]);

}