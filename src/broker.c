#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/socket.h"
#include "../include/defines.h"
#include "../include/logger.h"
#include "../include/SIG_Trap.h"
#include "../include/SignalHandler.h"

int main(int argc, char** argv) {
    pid_t pid = getpid();
    int in,out;
    Logger log = crearLogger();

    escribirLog(&log,DEBUG,pid,BROKER_NAME,"Hola soy el broker principal, voy a escuchar");

    int listenfd = abrir_socket_pasivo((char*) NULL,PORT_BROKER);
    int nfd = -1;

    char buffer[64];
    char inC[10];

    //Lanzo al router
    if (fork() == 0) {
        execl("./broker_router","./broker_router",(char*) NULL);
        perror("Exec fallo");
        return -1;
    }

    SIG_Trap SIGINT_trap;
    SIG_TrapInit(&SIGINT_trap, SIGINT);

    SignalHandlerRegisterHandler(SIGINT, &SIGINT_trap);
    while(SIG_TrapSignalWasReceived(&SIGINT_trap) == 0) {
        nfd = accept_socket(listenfd,false);

        if (nfd != -1) {
            escribirLog(&log,DEBUG,pid,BROKER_NAME,"Alguien se conectó");

            //Lanzo al que recibe msgs
            in = fork();

            if (in == 0) {
                close(listenfd);
                sprintf(buffer,"%d",nfd);
                execl("./broker_in","./broker_in",buffer,(char*) NULL);
                perror("Exec fallo");
                return -1;
            }

            //Lanzo al que manda msgs
            out = fork();

            if (out == 0) {
                close(listenfd);
                sprintf(inC,"%d",in);
                sprintf(buffer,"%d",nfd);
                execl("./broker_out","./broker_out",buffer,inC,(char*) NULL);
                perror("Exec fallo");
                return -1;
            }

            close(nfd);
        }

    }

    escribirLog(&log,DEBUG,pid,BROKER_NAME,"Cerrando el broker principal");

    cerrar_socket(listenfd);
    cerrarLogger(&log);
    return 0;
}

