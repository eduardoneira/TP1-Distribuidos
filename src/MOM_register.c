#include <stdbool.h>
#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/mensaje_registro.h"
#include "../include/rpc_wrapper.h"

void getHostRPC(char* ipRPC, char* pname){
    char nombre[20];
    char ip[20];
    char ipHost[20];

    FILE* fd = fopen(IPS,"r");

    while (feof(fd)) {
        fscanf(fd,"%s %s\n",nombre,ip);
        if (strcmp(pname,nombre) == 0){
            strcpy(ipHost,ip);
        }

        if(strcmp(nombre,RPC) == 0){
            strcpy(ipRPC,ip);
        }
    }

    fclose(fd);
    if (strcmp(ipRPC,ipHost) == 0){
        strcpy(ipRPC,LOCALHOST);
    }

}

int main(int argc, char** argv){
    Logger log = crearLogger();
    pid_t pid = getpid();
    char buffer[128];
    sprintf(buffer,"Hola soy el MOM para registrarse de %s",argv[1]);

    escribirLog(&log,TRACE,pid,MOM_REGISTER,buffer);

    Mensaje_registro msg;
    int msq_id = getmsgq(MSGQ_REGISTER_MOM);

    if (strcmp(argv[1],ALL) == 0) {
        strcpy(buffer,LOCALHOST);
    } else {
        getHostRPC(buffer,argv[1]);
    }


    CLIENT* clientRPC = initRPC(buffer);

    bool termine = false;

    while(!termine){
        if (recibirmsgqSinCheckeo(msq_id,&msg,sizeof(Mensaje_registro),MENSAJE_A_REGISTRO) == -1) {
            termine = true;
        } else {
            msg.mtype = msg.id;
            msg.id = getIdRPC(clientRPC);
            enviarmsgq(msq_id,&msg,sizeof(Mensaje_registro));
        }
    }

    destruirRPC(clientRPC);
    cerrarLogger(&log);

    return 0;
}