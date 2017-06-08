#ifndef MENSAJE_GUSTOS
#define MENSAJE_GUSTOS

#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

#define GUSTOS_POR_PERSONA 	3
#define MENSAJE_A_CAJERO	1
#define MENSAJE_A_HELADERO  -1
#define MENSAJE_IRSE		-2

typedef struct Mensaje_gustos {
	long mtype;
	int id;
	int momId;
	int gustos_helado[GUSTOS_POR_PERSONA];
} Mensaje_gustos;


void serializeMsgGusto(Mensaje_gustos* msg, char* buffer){
	char aux_buffer[10];
	sprintf(buffer,"%ld-%d-%d",msg->mtype,msg->id,msg->momId);

	for (int i = 0; i < GUSTOS_POR_PERSONA; i++) {
		sprintf(aux_buffer," %d",msg->gustos_helado[i]);
		strcat(buffer,aux_buffer);
	}
}

void deserializeMsgGusto(Mensaje_gustos* msg, char* buffer) {
	char aux[128];
	strcpy(aux,buffer);

	msg->mtype = atol(strtok(aux,SEPARATOR));
	msg->id = atoi(strtok(NULL,SEPARATOR));
	msg->momId = atoi(strtok(NULL,SEPARATOR));

	for (int i = 0; i < GUSTOS_POR_PERSONA; i++) {
		msg->gustos_helado[i] = atoi(strtok(NULL,SEPARATOR));
	}

}

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
    msg->momId = pid;
	msg->gustos_helado[0] = gusto1;
	msg->gustos_helado[1] = gusto2;
	msg->gustos_helado[2] = gusto3;
}

#endif