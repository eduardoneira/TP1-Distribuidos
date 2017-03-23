#ifndef LOGGER_H
#define LOGGER_H

#define DEBUG "DEBUG"
#define ERROR "ERROR"
#define BUFFER_SIZE 255

#include <stdio.h>
#include <string.h>
#include "lockfile.h"
#include "defines.h"

typedef struct Logger {
	int fd;
} Logger;

Logger crearLogger(){
	Logger log;
	log.fd = abrirLock(LOG_NAME);

	return log;
}

void initLogger(){

}

void escribir(Logger* log, char* modo, int pid, char* nombre_proceso ,char* message) {

	char buffer[BUFFER_SIZE];

	sprintf(buffer,"");
	
	tomarLock(log->fd);

	escribir(log->fd,buffer,sizeof(char)*strlen(buffer));

	liberarLock(log->fd);
}

void cerrarLogger(Logger* log) {
	cerrarLock(log->fd);
}


#endif