#include <stdbool.h>
#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/mensaje_registro.h"


//TODO: RPC
int main(int argc, char** argv){
    Logger log = crearLogger();
    pid_t pid = getpid();
    escribirLog(&log,TRACE,pid,MOM_REGISTER,"Hola soy el MOM para registrarse");

    Mensaje_registro msg;
    int msq_id = getmsgq(MSGQ_REGISTER_MOM);
    int id = 1;

    bool termine = false;

    while(!termine){
        if (recibirmsgqSinCheckeo(msq_id,&msg,sizeof(Mensaje_registro),MENSAJE_A_REGISTRO) == -1) {
            termine = true;
        } else {
            msg.mtype = msg.id;
            msg.id = id;
            enviarmsgq(msq_id,&msg,sizeof(Mensaje_registro));
            id++;
        }
    }

    cerrarLogger(&log);

    return 0;
}