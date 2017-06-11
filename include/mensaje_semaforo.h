
#ifndef TPS_DISTRIBUIDOS_MENSAJE_SEMAFORO_H
#define TPS_DISTRIBUIDOS_MENSAJE_SEMAFORO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

typedef struct Mensaje_semaforo {
    long mtype;
    int index;
} Mensaje_semaforo;

Mensaje_semaforo crearMsgSemaforo(long mtype, int index) {
    Mensaje_semaforo msg;
    msg.mtype = mtype;
    msg.index = index;
    return msg;
}

void serializeMsgSemaforo(Mensaje_semaforo* msg, char* buffer){
    sprintf(buffer,"%ld%s%d",msg->mtype,SEPARATOR,msg->index);
}

void deserializeMsgSemaforo(Mensaje_semaforo* msg, char* buffer){
    char aux[128];
    strcpy(aux,buffer);

    msg->mtype = atol(strtok(aux,SEPARATOR));
    msg->index = atoi(strtok(NULL,SEPARATOR));
}

#endif //TPS_DISTRIBUIDOS_MENSAJE_SEMAFORO_H
