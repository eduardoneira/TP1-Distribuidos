#ifndef MENSAJE_GUSTOS
#define MENSAJE_GUSTOS

#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

#define GUSTOS_POR_PERSONA 	3
#define MENSAJE_A_CAJERO	1
#define MENSAJE_A_HELADERO	1
#define MENSAJE_IRSE		-1

typedef struct Mensaje_gustos {
	long mtype;
	int id;
	int gustos_helado[GUSTOS_POR_PERSONA];
} Mensaje_gustos;


int esMsgDeIrse(Mensaje_gustos* msg){
	if (msg->id == MENSAJE_IRSE) {
		return 1;
	} else {
		return 0;
	}
}

void crearMsgIrse(Mensaje_gustos* msg){
	msg->id = MENSAJE_IRSE;
	msg->mtype = MENSAJE_A_CAJERO;
	msg->gustos_helado[0] = -1;
	msg->gustos_helado[1] = -1;
	msg->gustos_helado[2] = -1;
}

void crearMsgGustos(Mensaje_gustos* msg, long id, int pid, int gusto1, int gusto2, int gusto3) {
	msg->mtype = id;
	msg->id = pid;
	msg->gustos_helado[0] = gusto1;
	msg->gustos_helado[1] = gusto2;
	msg->gustos_helado[2] = gusto3;
}

#endif