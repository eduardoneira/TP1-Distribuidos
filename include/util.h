#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define DIRECTORY "."


key_t getkey(int id) {
	key_t clave;
	clave = ftok(DIRECTORY, id);
    if (clave == -1) {
    	perror("La clave ingresado es erronea");
    	exit(-1);
    }
    return clave;
}

#endif
