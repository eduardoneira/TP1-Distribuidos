#ifndef _FIFO_LECTURA_
#define _FIFO_LECTURA_

#include "fifo.h"

int abrirFifoLectura(char* nombre) {
	int fd;
	fd = open(nombre, O_RDONLY);
	
	if (fd == -1) {
		perror("Error al abrir fifo lectura");
		exit(-1);
	}

	return fd;
}

ssize_t leerFiFoLectura(int fd, void* buffer, const ssize_t buffsize) {
	ssize_t bytesrd = read(fd, buffer, buffsize);

	if (bytesrd == -1) {
		perror("Error al leer fifo");
		exit(-1);
	}
	return bytesrd;
}



#endif