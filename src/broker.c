#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/socket.h"
#include "../include/defines.h"

int main(int argc, char** argv) {
    int listenfd = abrir_socket_pasivo((char*) NULL,PORT_BROKER);
    int nfd = -1;
    char buffer[64];

    while ((nfd == accept_socket(listenfd,false)) != -1) {
        if (fork() == 0) {
            sprintf(buffer,"%d",nfd);
            execl("./workerBroker","./workerBroker",buffer,(char*) NULL);
        } else {
            close(nfd);
        }
    }

    return 0;
}

