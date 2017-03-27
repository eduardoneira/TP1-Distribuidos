#ifndef DESTRUCTOR_H
#define DESTRUCTOR_H

#define DESTRUCTOR "DESTRUCTOR"

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
	v(semid,0);

	if (shmid == -1) {
		perror("Error al conseguir la memoria compartida");
		exit(ERROR_DESTRUIR_IPC);
	}

	if (elishm(shmid) == -1) {
		perror("Error al eliminar la shm");
		exit(ERROR_DESTRUIR_IPC);
	}

	

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

void avisarQueSeCierra(int idmsq) {
	//Time to TCP
	Mensaje_gustos msg;
	crearMsgIrse(&msg);
	enviarmsgq(idmsq,&msg,sizeof(Mensaje_gustos));

	recibirmsgq(idmsq,&msg,sizeof(Mensaje_gustos),MENSAJE_A_MANAGER);

}

void destruirMsgQueues() {
	int idmsq = getmsgq(MSGQ_CLIENTES_AL_CAJERO);

	if (idmsq == -1) {
		perror("Error al conseguir cola 1");
		exit(ERROR_DESTRUIR_IPC);
	}

	avisarQueSeCierra(idmsq);

	if (elimsgq(idmsq) == -1) {
		perror("Error al destruir cola 1");
		exit(ERROR_DESTRUIR_IPC);
	}

	idmsq = getmsgq(MSGQ_CAJERO_A_HELADEROS);

	if (idmsq == -1) {
		perror("Error al conseguir cola 2");
		exit(ERROR_DESTRUIR_IPC);
	}

	avisarQueSeCierra(idmsq);
	avisarQueSeCierra(idmsq);

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


int destruirIPC(int params) {
	pid_t pid = getpid();
	
	Logger log = crearLogger();
	
	if (destruirEstadoHeladeria() != 0 && params == 1) {
		escribirLog(&log,ERROR,pid,DESTRUCTOR,"La tienda tiene que estar cerrada y no debe haber clientes");
		cerrarLogger(&log);
		return 1;
	}
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo el estado de heladeria");
	
	destruirMsgQueues();
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo las colas de msgs para comunicación");

	destruirGustosHelados();
	escribirLog(&log,DEBUG,pid,DESTRUCTOR,"Se destruyo los gustos de helado");

	cerrarLogger(&log);

	return 0;
}

#endif