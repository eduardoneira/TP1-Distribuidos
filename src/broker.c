#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/socket.h"
#include "../include/defines.h"
#include "../include/logger.h"

int main(int argc, char** argv) {
    pid_t pid = getpid();
    int cpid;
    Logger log = crearLogger();

    escribirLog(&log,DEBUG,pid,BROKER,"Hola soy el broker principal, voy a escuchar");

    int listenfd = abrir_socket_pasivo((char*) NULL,PORT_BROKER);
    int nfd = -1;

    char buffer[64];

    //Lanzo al router
    if (fork() == 0) {
        execl("./broker_router","./broker_router",(char*) NULL);
        perror("Exec fallo");
        return -1;
    }

    while ((nfd == accept_socket(listenfd,false)) != -1) {

        escribirLog(&log,DEBUG,pid,BROKER,"Alguien se conectó");
        //Lanzo al que recibe msgs
        cpid = fork();

        if (cpid == 0) {
            sprintf(buffer,"%d",nfd);
            execl("./broker_in","./broker_in",buffer,(char*) NULL);
            perror("Exec fallo");
            return -1;
        } else {
            close(nfd);
        }
        
        //Lanzo al que recibe msgs        
        cpid = fork();

        if (cpid == 0) {
            sprintf(buffer,"%d",nfd);
            execl("./broker_out","./broker_out",buffer,(char*) NULL);
            perror("Exec fallo");
            return -1;
        } else {
            close(nfd);
        }
    }

    escribirLog(&log,DEBUG,pid,BROKER,"Cerrando el broker principal");

    cerrar_socket(listenfd);
    cerrarLogger(&log);
    return 0;
}

