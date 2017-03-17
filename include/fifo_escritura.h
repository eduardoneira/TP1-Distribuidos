#ifndef _FIFO_ESCRITURA_
#define _FIFO_ESCRITURA_

#include "fifo.h"

int abrirFifoEscritura(char* nombre) {
	int fd;
	fd = open(nombre, O_WRONLY);
	
	if (fd == -1) {
		perror("Error al abrir fifo lectura");
		exit(-1);
	}

	return fd;
}

ssize_t escribirFifo(int fd, void* buffer, const ssize_t buffsize) {
	ssize_t byteswr = write(fd, buffer, buffsize);

	if (byteswr == -1) {
		perror("Error al leer fifo");
		exit(-1);
	}

	return byteswr;
}



#endif