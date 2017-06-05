#include <stdio.h>

#include "../include/msg_queue.h"
#include "../include/message_wrapper.h"
#include "../include/logger.h"
#include "../include/socket.h"


/**
 * Args
 * char*    nombre
 * int      fd del socket
 * int      mtype para recibir del msq
 * */
int main(int argc, char** argv){
    if (argc != 3) {
        printf("Deberían venir 3 argumentos");
        return 1;
    }

    //Log inicial
    Logger log = crearLogger();
    pid_t pid = getpid();
    char buffer[64];

    sprintf(buffer,"Soy el broker out para el socket %s leyendo con mype %s",argv[1],argv[2]);
    escribirLog(&log,DEBUG,pid,BROKER_OUT_NAME,buffer);

    //IPC
    int msqid = getmsgq(MSGQ_ROUTER_BROKER_OUT);
    int socket = atoi(argv[1]);

    //Variables locales
    bool termine = false;
    Message msg;
    MessageQ msgq;
    int mtype = atoi(argv[2]);

    while (!termine) {
        if (recibirmsgqSinCheckeo(msqid,&msgq,sizeof(MessageQ),mtype) == -1) {
            termine = true;
        } else {
            crearMessage(&msg,&msgq);
            escribirLog(&log,DEBUG,pid,BROKER_OUT_NAME,"Me llego un msg del router, lo mando por el socket");
            escribir_socket(socket,(void*) &msg,sizeof(Message));

            if (esMsgDesregistrarse(msg)) {
                termine = true;
            }
        }
    }

    escribirLog(&log,DEBUG,pid,BROKER_OUT_NAME,"Cerrando broker out");

    cerrarLogger(&log);
    cerrar_socket(socket);

    return 0;
}
