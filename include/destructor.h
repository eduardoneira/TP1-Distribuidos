#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

#include <stdbool.h>
#include "semaforo.h"
#include "memoria_compartida.h"
#include "estado_heladeria.h"
#include "defines.h"
#include "logger.h"
#include "msg_queue.h"
#include "mensaje_gustos.h"

int destruirEstadoHeladeria() {
	int shmid = getshm(ID_SHM_ESTADO_HELADERIA);
	int semid = getsem(SEMID_ESTADO_HELADERIA,1);

	p(semid,0);
	EstadoHeladeria* estado = (EstadoHeladeria*) map(shmid);
	if(estado->tamanio_cola != TAMANIO_COLA || estado->tamanio_heladeria != TAMANIO_HELADERIA || estado->abierto != CERRADO){
		unmap(estado);
		v(semid,0);
		return -1;
	}
	unmap(estado);


	if (shmid == -1) {
		perror("Error al conseguir la memoria compartida");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elishm(shmid) == -1) {
		perror("Error al eliminar la shm");
		exit(ERROR_DESTRUIR_IPC);
	}

	v(semid,0);

	if (semid == -1) {
		perror("Error al conseguir semaforo de estado");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elisem(semid, 0) == -1) {
		perror("Error al destruir semaforo de estado");
		exit(ERROR_DESTRUIR_IPC);
	}

	return 0;

}

void destruirCola(int id, char* error, int cant_avisar) {
	int idmsq = getmsgq(id);

	if (elimsgq(idmsq) == -1) {
		sprintf(error,"Error al destruir cola con id %d",id);
		perror(error);
		exit(ERROR_DESTRUIR_IPC);
	}

}

void destruirMsgQueues(char* modo) {

	char error[64];

	if (strcmp(modo,ALL) == 0 || strcmp(modo,CAJERO) == 0) {
		destruirCola(MSGQ_RECIBIR_CAJERO,error,0);
		destruirCola(MSGQ_POR_MOMID_CAJERO,error,0);
		destruirCola(MSGQ_POR_PID_CAJERO,error,0);
	}

	if (strcmp(modo,ALL) == 0 || strcmp(modo,HELADERO) == 0) {
		destruirCola(MSGQ_RECIBIR_HELADERO,error,0);
		destruirCola(MSGQ_POR_MOMID_HELADERO,error,0);
		destruirCola(MSGQ_POR_PID_HELADERO,error,0);
	}

	if (strcmp(modo,ALL) == 0 || strcmp(modo,CLIENTE) == 0) {
		destruirCola(MSGQ_RECIBIR_CLIENTE,error,0);
		destruirCola(MSGQ_POR_PID_CLIENTE,error,0);
		destruirCola(MSGQ_POR_MOMID_CLIENTE,error,0);
		destruirCola(MSGQ_POR_TICKET,error,0);
	}

	if (strcmp(modo,BROKER) == 0){
		destruirCola(MSGQ_ROUTER_BROKER_OUT,error,0);
		destruirCola(MSGQ_BROKER_IN_ROUTER,error,0);
	}

}

void destruirGustosHelados() {
	int i;

	for (i = PRIMER_ID_GUSTO_HELADO; i < PRIMER_ID_GUSTO_HELADO + CANTIDAD_GUSTOS;i++) {

		int semid = getsem(i,1);

		if (semid == -1) {
			perror("Error al conseguir semaforo de gusto de helado");
			exit(ERROR_DESTRUIR_IPC);
		}

		if (elisem(semid,0) == -1){
			perror("Error al destruir semaforo de gusto de helado");
			exit(ERROR_DESTRUIR_IPC);
		}
	}
}


int destruirIPC(char* modo) {
	pid_t pid = getpid();
	
	Logger log = crearLogger();
	
	/*if ((strcmp(modo,CLIENTE) == 0 || strcmp(modo,ALL) == 0) && destruirEstadoHeladeria() != 0) {
		escribirLog(&log,ERROR,pid,DESTRUCTOR,"La tienda tiene que estar cerrada y no debe haber clientes");
		cerrarLogger(&log);
		return 1;
	}
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo el estado de heladeria");
	*/

	destruirMsgQueues(modo);
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo las colas de msgs para comunicación");
	
	/*
	if (strcmp(modo,HELADERO) == 0 || strcmp(modo,ALL) == 0) {
		destruirGustosHelados();
		escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo los gustos de helado");
	}
	*/

	cerrarLogger(&log);

	return 0;
}

#endif