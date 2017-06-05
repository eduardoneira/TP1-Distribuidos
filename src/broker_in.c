#include <stdio.h>

#include "../include/msg_queue.h"
#include "../include/message_wrapper.h"
#include "../include/logger.h"
#include "../include/socket.h"

/**
 * Args
 * char*    nombre
 * int      fd del socket
 * */

int main(int argc, char** argv){
    if (argc != 2) {
        printf("Deberían venir 2 argumentos");
        return 1;
    }

    //Log inicial
    Logger log = crearLogger();
    pid_t pid = getpid();
    char buffer[64];

    sprintf(buffer,"Soy el broker in de el socket %s",argv[1]);
    escribirLog(&log,DEBUG,pid,BROKER_IN_NAME,buffer);

    //IPC
    int msqid = getmsgq(MSGQ_BROKER_IN_ROUTER);
    int socket = atoi(argv[1]);

    //Variables locales
    bool termine = false;
    Message msg;
    MessageQ msgq;

    while (!termine) {
        if (leer_socket(socket,&msg,sizeof(Message)) == -1) {
            termine = true;
        } else {
            escribirLog(&log,DEBUG,pid,BROKER_IN_NAME,"Me llego un msg del socket, lo mando al router");

            crearMessageQ(&msg,&msgq,pid);

            enviarmsgq(msqid,(void*) &msgq,sizeof(MessageQ));

            if (esMsgCerrar(msg)) {
                termine = true;
            }
        }
    }

    escribirLog(&log,DEBUG,pid,BROKER_IN_NAME,"Cerrando broker in");

    cerrarLogger(&log);
    cerrar_socket(socket);

    return 0;
}

