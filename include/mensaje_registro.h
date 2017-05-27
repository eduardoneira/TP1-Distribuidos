#ifndef TPS_DISTRIBUIDOS_MENSAJE_REGISTRO_H
#define TPS_DISTRIBUIDOS_MENSAJE_REGISTRO_H

#include "msg_queue.h"
#define MENSAJE_A_REGISTRO 1

typedef struct Mensaje_registro {
    long mtype;
    int id;
} Mensaje_registro;

Mensaje_registro crearMensajeRegistro(int id) {
    Mensaje_registro msg;
    msg.mtype = MENSAJE_A_REGISTRO;
    msg.id = id;
    return msg;
}

void serializeMsgRegistro(Mensaje_registro* msg, char* buffer){
    sprintf(buffer,"%ld %d",msg->mtype,msg->id);
}

void deserializeMsgRegistro(Mensaje_registro* msg, char* buffer) {
    char aux[128];
    strcpy(aux,buffer);

    msg->mtype = atol(strtok(aux,SEPARATOR));
    msg->id = atoi(strtok(NULL,SEPARATOR));
}

bool esMsgCerrar(Mensaje_registro msg) {
    return (msg.id == -1);
}

int registrarse() {
    int pid = getpid();
    Mensaje_registro msg = crearMensajeRegistro(pid);
    int reg_id = getmsgq(MSGQ_REGISTER_MOM);
    enviarmsgq(reg_id,&msg,sizeof(Mensaje_registro));
    recibirmsgq(reg_id,&msg,sizeof(Mensaje_registro),pid);
    return msg.id;
}

#endif //TPS_DISTRIBUIDOS_MENSAJE_REGISTRO_H
