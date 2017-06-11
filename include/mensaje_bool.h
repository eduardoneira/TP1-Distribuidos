
#ifndef TPS_DISTRIBUIDOS_MENSAJE_BOOL_H
#define TPS_DISTRIBUIDOS_MENSAJE_BOOL_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"

typedef struct Mensaje_bool {
    long mtype;
    bool estado;
} Mensaje_bool;

void serializeMsgBool(Mensaje_bool* msg, char* buffer){
    sprintf(buffer,"%ld-%d",msg->mtype,msg->estado);
}

void deserializeMsgBool(Mensaje_bool* msg, char* buffer){
    char aux[128];
    strcpy(aux,buffer);

    msg->mtype = atol(strtok(buffer,SEPARATOR));
    msg->estado = atoi(strtok(NULL,SEPARATOR));
}

Mensaje_bool crearMensajeBool(long mtype,bool estado) {
    Mensaje_bool msg;
    msg.mtype = mtype;
    msg.estado = estado;
    return msg;
}
#endif //TPS_DISTRIBUIDOS_MENSAJE_BOOL_H
