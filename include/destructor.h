#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

#define DESTRUCTOR "DESTRUCTOR"

#include "semaforo.h"
#include "memoria_compartida.h"
#include "estado_heladeria.h"
#include "defines.h"
#include "logger.h"
#include "msg_queue.h"

void destruirEstadoHeladeria() {
	int shmid = getshm(ID_SHM_ESTADO_HELADERIA);

	if (shmid == -1) {
		perror("Error al conseguir la memoria compartida");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elishm(shmid) == -1) {
		perror("Error al eliminar la shm");
		exit(ERROR_DESTRUIR_IPC);
	}

	int semid = getsem(SEMID_ESTADO_HELADERIA,1);

	if (semid == -1) {
		perror("Error al conseguir semaforo de estado");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elisem(semid, 0) == -1) {
		perror("Error al destruir semaforo de estado");
		exit(ERROR_DESTRUIR_IPC);
	}

}

void destruirMsgQueues() {
	int idmsq = getmsgq(MSGQ_CLIENTES_AL_CAJERO);

	if (idmsq == -1) {
		perror("Error al conseguir cola 1");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elimsgq(idmsq) == -1) {
		perror("Error al destruir cola 1");
		exit(ERROR_DESTRUIR_IPC);
	}

	idmsq = getmsgq(MSGQ_CAJERO_A_HELADEROS);

	if (idmsq == -1) {
		perror("Error al conseguir cola 2");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elimsgq(idmsq) == -1) {
		perror("Error al destruir cola 2");
		exit(ERROR_DESTRUIR_IPC);
	}

	idmsq = getmsgq(MSGQ_HELADEROS_A_CLIENTES);


	if (idmsq == -1) {
		perror("Error al conseguir cola 3");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elimsgq(idmsq) == -1) {
		perror("Error al destruir cola 3");
		exit(ERROR_DESTRUIR_IPC);
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


void destruirIPC() {
	pid_t pid = getpid();
	
	Logger log = crearLogger();
	char msg_log[BUFFER_SIZE];
	
	destruirEstadoHeladeria();
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo el estado de heladeria");
	
	destruirMsgQueues();
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo las colas de msgs para comunicación");

	destruirGustosHelados();
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo los gustos de helado");

	cerrarLogger(&log);
}

#endif