#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void crearFifo(const char* nombre) {
	if(mknod(nombre,S_IFIFO|0666,0 ) == -1){
        perror("Error al crear fifo");
        exit(-1);
	}
}

void cerrarFifo(int fd) {
	if (close(fd) == -1) {
		perror("Error al cerrar fifo");
		exit(-1);
	}
}

void eliminarFifo(const char* nombre) {
	if(unlink ( nombre ) == -1){
        perror("Error al eliminar fifo ");
	}
}


#endif