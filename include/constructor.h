#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include "semaforo.h"
#include "memoria_compartida.h"
#include "estado_heladeria.h"
#include "defines.h"
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

void crearMsgQueue() {
	
}

void crearIPC() {
	crearEstadoHeladeria();

}

#endif