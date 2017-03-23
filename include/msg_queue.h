#ifndef _MSG_QUEUE_H_
#define _MSG_QUEUE_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "util.h"

int crearmsgq(int id){
    key_t clave;
    clave = getkey(id);
    return (msgget(clave,  IPC_CREAT | IPC_EXCL | 0660));
    /* da error si ya existe */
}

int getmsgq(int id){
    key_t clave;
    clave = getkey(id);
    return (msgget(clave, 0660));
}

void enviarmsgq(int id, const void *msgp, size_t msgsz){
    if(msgsnd(id,msgp,msgsz-sizeof(long),0)==-1){
        perror("No se puede enviar el mensaje");
        exit(-1);
    }
}

void recibirmsgq(int id, void *msgp, size_t msgsz, long type){
    if(msgrcv(id,msgp,msgsz-sizeof(long),type,0)==-1){
        perror("No se puede recibir el mensaje");
        exit(-1);
    }
}

int elimsgq(int id){
    return (msgctl(id, IPC_RMID, NULL));
}

#endif /* _MSG_H_ */
