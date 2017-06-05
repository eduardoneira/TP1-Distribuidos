#include <stdio.h>

#include "../include/msg_queue.h"
#include "../include/message_wrapper.h"
#include "../include/logger.h"
#include "../include/socket.h"

int main(int argc, char** argv){
    if (argc != 2) {
        printf("Deberían venir 2 argumentos");
        return 1;
    }

    //Log inicial
    Logger log = crearLogger();
    pid_t pid = getpid();
    char buffer[64];

    sprintf(buffer,"Soy el broker out para el socket %s",argv[1]);
    escribirLog(&log,DEBUG,pid,BROKER_OUT,buffer);

    //IPC
    int msqid = getmsgq(MSGQ_ROUTER_BROKER_OUT);
    int socket = atoi(argv[1]);

    //Variables locales
    bool termine = false;
    Message msg;
    MessageQ msgq;

    while (!termine) {
        if (recibirmsgqSinCheckeo(msqid,&msgq,sizeof(MessageQ),pid) == -1) {
            termine = true;
        } else {
            crearMessage(&msg,&msgq);
            escribirLog(&log,DEBUG,pid,BROKER_OUT,"Me llego un msg del router, lo mando por el socket");
            escribir_socket(socket,(void*) &msg,sizeof(Message));

            if (esMsgDesregistrarse(msg)) {
                termine = true;
            }
        }
    }

    escribirLog(&log,DEBUG,pid,BROKER_OUT,"Cerrando broker out");

    cerrarLogger(&log);
    cerrar_socket(socket);

    return 0;
}
