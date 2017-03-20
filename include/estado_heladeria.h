#ifndef _ESTADO_HELADERIA_H_
#define _ESTADO_HELADERIA_H_

#include "semaforo.h"
#include "memoria_compartida.h"
#include "defines.h"


typedef struct Estado {
	int asientos_libres;
	int lugares_libre_cola;
	int cantidad_gustos;
} Estado;

typedef struct EstadoHeladeria {
	int semid;
	int id_shm;
	Estado* estado;
} EstadoHeladeria;


EstadoHeladeria crearEstadoHeladeria(int N, int M) {
	EstadoHeladeria heladeria;
	heladeria.id_shm = crearshm(ID_SHM_ESTADO_HELADERIA,sizeof(Estado));
	
	int semid = crearsem(SEMID_ESTADO_HELADERIA, 1);
	inisem(semid, 0, 1);
	
	heladeria.semid = semid;
	heladeria.estado = (Estado*) -1;

	return heladeria;
}

Estado getEstado(int N, int M) {
	Estado estado;
	estado.asientos_libres = N;
	estado.lugares_libre_cola = M;
}

EstadoHeladeria getEstadoHeladeria() {
	EstadoHeladeria heladeria;
	heladeria.semid = getsem(ID_SHM_ESTADO_HELADERIA,1);
	heladeria.id_shm = getshm(SEMID_ESTADO_HELADERIA);
	heladeria.estado = (Estado*) -1;

	return heladeria;
}

EstadoHeladeria leerEstadoHeladeria(EstadoHeladeria heladeria) {
	p(heladeria.semid,0);
	heladeria.estado = (Estado*) map(heladeria.id_shm);
	v(heladeria.semid,0);

	return heladeria;
}

EstadoHeladeria escribirEstadoHeladeria(EstadoHeladeria heladeria, Estado estado) {
	p(heladeria.semid,0);
	if (heladeria.estado == (Estado*) -1) {
		leerEstadoHeladeria(heladeria);
	}
	heladeria.estado = &estado;
	v(heladeria.semid,0);

	return heladeria;
}

void   detachEstadoHeladeria(EstadoHeladeria heladeria){
	unmap((void*)heladeria.estado);

}


void eliminarEstadoHeladeria(EstadoHeladeria heladeria){
	elishm(heladeria.id_shm);
	elisem(heladeria.semid,0);
}

#endif