#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include <errno.h>
#include "semaforo.h"
#include "memoria_compartida.h"
#include "estado_heladeria.h"
#include "defines.h"
#include "logger.h"
#include "msg_queue.h"

void crearEstadoHeladeria() {

	//Creo memoria compartida
	int shmid = crearshm(ID_SHM_ESTADO_HELADERIA,sizeof(EstadoHeladeria));
	
	if (shmid == -1) {
		perror("Error al crear la memoria compartida");
		exit(ERROR_CREAR_IPC);
	}

	EstadoHeladeria* estadoInicial = (EstadoHeladeria*) map(shmid);

	estadoInicial->tamanio_cola = TAMANIO_COLA;
	estadoInicial->tamanio_heladeria = TAMANIO_HELADERIA;
	estadoInicial->abierto = CERRADO;

	if (unmap((void *)estadoInicial) == -1) {
		perror("Error al hacer el detach de la memeria");
		exit(ERROR_CREAR_IPC);
	}

	//Creo semaforo

	int semid = crearsem(SEMID_ESTADO_HELADERIA,1);

	if (semid == -1) {
		perror("Error al crear semaforo");
		exit(ERROR_CREAR_IPC);
	}

	if (inisem(semid,0,1) == -1){
		perror("Error al inicializar semaforo");
		exit(ERROR_CREAR_IPC);
	}
}

void crearColasDeMsgs(char* modo) {

	if (strcmp(modo,ALL) == 0 || strcmp(modo,CLIENTE) == 0){
		if (crearmsgq(MSGQ_PASAMANOS_CLIENTE_MOM_PEDIDO) == -1 || crearmsgq(MSGQ_PASAMANOS_MOM_CLIENTE_TICKET) == -1 || crearmsgq(MSGQ_PASAMANOS_MOM_CLIENTE_HELADO) == -1) {
			perror("Error al crear colas de mensajes de cliente");
			exit(ERROR_CREAR_IPC);
		}
	}
	if (strcmp(modo,ALL) == 0 || strcmp(modo,HELADERO) == 0){
		if (crearmsgq(MSGQ_PASAMANOS_MOM_HELADERO_PEDIDO) == -1 || crearmsgq(MSGQ_PASAMANOS_HELADERO_MOM_HELADO) == -1) {
			perror("Error al crear colas de mensajes de heladero");
			exit(ERROR_CREAR_IPC);
		}
	}

	if (strcmp(modo,ALL) == 0 || strcmp(modo,CAJERO) == 0) {
		if (crearmsgq(MSGQ_PASAMANOS_CAJERO_MOM_PEDIDO) == -1 || crearmsgq(MSGQ_PASAMANOS_MOM_CAJERO_PEDIDO) == -1 || crearmsgq(MSGQ_PASAMANOS_CAJERO_MOM_TICKET) == -1) {
			perror("Error al crear colas de mensajes de cliente");
			exit(ERROR_CREAR_IPC);
		}
	}

	if (strcmp(modo,BROKER) != 0) {
		if (crearmsgq(MSGQ_REGISTER_MOM) == -1 && errno != EEXIST) {
			perror("Error al crear colas de mensajes para registro");
			exit(ERROR_CREAR_IPC);
		}

		if (crearmsgq(MSGQ_DESTRUCTOR) == -1 && errno != EEXIST) {
			perror("Error al crear colas de mensajes para destruir");
			exit(ERROR_CREAR_IPC);
		}
	} else {
		if (crearmsgq(MSGQ_BROKER_IN_ROUTER) == -1 || crearmsgq(MSGQ_ROUTER_BROKER_OUT) == -1) {
			perror("Error al crear colas de mensajes de broker");
			exit(ERROR_CREAR_IPC);
		}
	}
}

void crearHelados() {
	int i;

	for (i = PRIMER_ID_GUSTO_HELADO; i < PRIMER_ID_GUSTO_HELADO + CANTIDAD_GUSTOS;i++) {
		//Creo semaforo

		int semid = crearsem(i,1);

		if (semid == -1) {
			perror("Error al crear semaforo");
			exit(ERROR_CREAR_IPC);
		}

		if (inisem(semid,0,1) == -1){
			perror("Error al inicializar semaforo");
			exit(ERROR_CREAR_IPC);
		}
	}

}

void crearIPC(char* modo) {
	pid_t pid = getpid();
	
	Logger log = crearLogger();
	initLogger(&log);
	/*
	if (strcmp(modo,ALL) == 0 || strcmp(modo,CLIENTE) == 0) {
		crearEstadoHeladeria();
		escribirLog(&log,DEBUG,pid,CONSTRUCTOR,"Se creo el estado de heladeria");
	}*/

	crearColasDeMsgs(modo);
	escribirLog(&log,DEBUG,pid,CONSTRUCTOR,"Se crearon las colas de msgs para comunicación");
	/*
	if (strcmp(modo,ALL) == 0 || strcmp(modo,HELADERO) == 0) {
		crearHelados();
		escribirLog(&log,DEBUG,pid,CONSTRUCTOR,"Se crearon los gustos de helado");
	}*/

	cerrarLogger(&log);
}

#endif