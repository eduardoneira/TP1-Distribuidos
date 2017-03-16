#ifndef _MEMORIA_COMPARTIDA_H_
#define _MEMORIA_COMPARTIDA_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "util.h"

int   getshm(int);
int   creashm(int,int);
void* map(int);
int   unmap(void*);
int   elishm(int);

int   creashm(int id, int size){
    key_t clave;
    clave = getkey(id);
    return (shmget(clave, size, IPC_CREAT | IPC_EXCL | 0660));
}

int getshm(int id){
    key_t clave;
    clave = getkey(id);
    return (shmget(clave, 1, 0660));
}

void* map(int id){
    void* addr = shmat(id, NULL, 0);
    if(addr == (void*)-1){
        perror("No se pudo mapear la memoria");
        exit(-1);
    }
    return addr;
}

int   unmap(void* addr){
    return shmdt(addr);
}

int   elishm(int id){
    return (shmctl(id, IPC_RMID, (struct shmid_ds *) 0));
}

#endif