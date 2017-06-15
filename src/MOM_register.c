#include <stdbool.h>
#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/mensaje_registro.h"
#include "../include/rpc_wrapper.h"


int main(int argc, char** argv){
    Logger log = crearLogger();
    pid_t pid = getpid();
    char buffer[128];
    char host[20];
    sprintf(buffer,"Hola soy el MOM para registrarse de %s",argv[1]);

    escribirLog(&log,TRACE,pid,MOM_REGISTER,buffer);

    Mensaje_registro msg;
    int msq_id = getmsgq(MSGQ_REGISTER_MOM);

    if (strcmp(argv[1],ALL) == 0) {
        strcpy(host,LOCALHOST);
    } else {
        getHostRPC(host,argv[1]);
    }

    sprintf(buffer,"Conectando a servidor RPC en %s",host);
    escribirLog(&log,TRACE,pid,MOM_REGISTER,buffer);
    CLIENT* clientRPC = initRPC(host);

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