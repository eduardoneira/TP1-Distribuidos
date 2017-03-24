#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/logger.h"


#define CLIENTE "CLIENTE"

int main(int argc, char** argv) {

	pid_t pid = getpid();

	Logger log = crearLogger();
	
	escribirLog(&log,DEBUG,pid,CLIENTE,"Hola soy un cliente");

	cerrarLogger(&log);

	return 0;
}