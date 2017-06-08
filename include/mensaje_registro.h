#ifndef TPS_DISTRIBUIDOS_MENSAJE_REGISTRO_H
#define TPS_DISTRIBUIDOS_MENSAJE_REGISTRO_H

#include "msg_queue.h"
#include "message_wrapper.h"

#define MENSAJE_A_REGISTRO 1

typedef struct Mensaje_registro {
    long mtype;
    int id;
    char* tipo;
} Mensaje_registro;

Mensaje_registro crearMensajeRegistro(int id,const char* tipo) {
    Mensaje_registro msg;
    msg.mtype = MENSAJE_A_REGISTRO;
    msg.id = id;
    strcpy(msg.tipo,tipo);
    return msg;
}

void serializeMsgRegistro(Mensaje_registro* msg, char* buffer){
    sprintf(buffer,"%ld-%d-%s",msg->mtype,msg->id,msg->tipo);
}

void deserializeMsgRegistro(Mensaje_registro* msg, char* buffer) {
    char aux[128];
    strcpy(aux,buffer);

    msg->mtype = atol(strtok(aux,SEPARATOR));
    msg->id = atoi(strtok(NULL,SEPARATOR));
    msg->tipo = strtok(NULL,SEPARATOR);
}

int registrarse(const char* tipo) {
    int pid = getpid();
    Mensaje_registro msg_reg = crearMensajeRegistro(pid,tipo);
    int reg_id = getmsgq(MSGQ_POR_PID);
    MessageQ msg;

    sprintf(msg.type,"%d",MSG_BROKER_REGISTER);
    msg.mtype = 1;
    serializeMsgRegistro(&msg_reg,msg.payload);

    enviarmsgq(reg_id,&msg,sizeof(MessageQ));
    recibirmsgq(reg_id,&msg,sizeof(MessageQ),pid);
    deserializeMsgRegistro(&msg_reg,msg.payload);

    return msg_reg.id;
}

void desregistrarse(int msgq, int momId,const char* tipo) {
    Mensaje_registro msg_reg = crearMensajeRegistro(momId,tipo);
    MessageQ msg;

    sprintf(msg.type,"%d",MSG_BROKER_DESREGISTRARSE);
    msg.mtype = 1;
    serializeMsgRegistro(&msg_reg,msg.payload);
    enviarmsgq(msgq,&msg,sizeof(MessageQ));
}

#endif //TPS_DISTRIBUIDOS_MENSAJE_REGISTRO_H
