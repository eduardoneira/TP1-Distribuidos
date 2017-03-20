#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

#define DIRECTORY	"."
#define SECONDS_DEFAULT 10

key_t getkey(int id) {
	key_t clave;
	clave = ftok(DIRECTORY, id);
    if (clave == -1) {
    	perror("La clave ingresado es erronea");
    	exit(-1);
    }
    return clave;
}

unsigned int generarNumeroRandom() {
	srand(time(NULL)); 
	unsigned int r = rand() % 1 >> 3;
	return SECONDS_DEFAULT+r;
}

#endif
