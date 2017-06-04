#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/socket.h"
#include "../include/defines.h"
#include "../include/Logger.h"

int main(int argc, char** argv) {
    pid_t pid = getpid();
    Logger log = crearLogger();

    escribirLog(&log,DEBUG,pid,BROKER,"Hola soy el broker principal");

    int listenfd = abrir_socket_pasivo((char*) NULL,PORT_BROKER);
    int nfd = -1;
    char buffer[64];
    int cpid;

    //Lanzo al router
    if (fork() == 0) {

    }

    while ((nfd == accept_socket(listenfd,false)) != -1) {
        
        //Lanzo al que recibe msgs
        cpid = fork();

        if (cpid == 0) {
            sprintf(buffer,"%d",nfd);
            execl("./broker_in","./broker_in",buffer,(char*) NULL);
        } else {
            close(nfd);
        }
        
        //Lanzo al que recibe msgs        
        cpid = fork();

        if (cpid == 0) {
            sprintf(buffer,"%d",nfd);
            execl("./broker_out","./broker_out",buffer,(char*) NULL);
        } else {
            close(nfd);
        }
    }

    cerrar_socket(listenfd);

    return 0;
}

