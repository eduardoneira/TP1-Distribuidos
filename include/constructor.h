#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#define CONSTRUCTOR "CONSTRUCTOR"

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
	estadoInicial->abierto = ABIERTO;

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

void crearColasDeMsgs() {
	if (crearmsgq(MSGQ_CLIENTES_AL_CAJERO) == -1 || crearmsgq(MSGQ_CAJERO_A_HELADEROS) == -1 || crearmsgq(MSGQ_HELADEROS_A_CLIENTES) == -1) {
		perror("Error al crear colas de mensajes");
		exit(ERROR_CREAR_IPC);
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

void crearIPC() {
	pid_t pid = getpid();
	
	Logger log = crearLogger();
	initLogger(&log);
	
	crearEstadoHeladeria();
	escribirLog(&log,DEBUG,pid,CONSTRUCTOR,"Se creo el estado de heladeria");
	
	crearColasDeMsgs();
	escribirLog(&log,DEBUG,pid,CONSTRUCTOR,"Se crearon las colas de msgs para comunicación");

	crearHelados();
	escribirLog(&log,DEBUG,pid,CONSTRUCTOR,"Se crearon los gustos de helado");

	cerrarLogger(&log);
}

#endif