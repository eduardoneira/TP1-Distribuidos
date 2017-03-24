#ifndef LOGGER_H
#define LOGGER_H

#define DEBUG "DEBUG"
#define ERROR "ERROR"
#define BUFFER_SIZE 255
#define TIME_SIZE	28

#include <stdio.h>
#include <string.h>
#include "lockfile.h"
#include "defines.h"
#include "util.h"

typedef struct Logger {
	int fd;
} Logger;

Logger crearLogger(){
	Logger log;
	log.fd = abrirLock(LOG_NAME);

	return log;
}

void initLogger(Logger* log){
	char buffer[BUFFER_SIZE];

	strcpy(buffer,"--------------------------------------COMIENZA EL LOG--------------------------------------\n");

	tomarLock(log->fd);

	escribir(log->fd,buffer,sizeof(char)*strlen(buffer));

	liberarLock(log->fd);	
}

void escribirLog(Logger* log, const char* modo, int pid,const char* nombre_proceso , const char* message) {

	char buffer[BUFFER_SIZE];

	char time[TIME_SIZE];

	get_timestamp(time,TIME_SIZE);

	sprintf(buffer,"%s \t %s \t %d \t %s \t %s \n",modo,time,pid,nombre_proceso,message);
	
	tomarLock(log->fd);

	escribir(log->fd,buffer,sizeof(char)*strlen(buffer));

	liberarLock(log->fd);
}

void cerrarLogger(Logger* log) {
	cerrarLock(log->fd);
}

void eliminarLogger() {
	eliminarLock(LOG_NAME);	
}


#endif