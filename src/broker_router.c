#include <stdio.h>
#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/message_wrapper.h"

int main(int argc, char** argv){
    //Log inicial
    Logger log = crearLogger();
    pid_t pid = getpid();
    char buffer[64];

    sprintf(buffer,"Soy un router broker");
    escribirLog(&log,DEBUG,pid,BROKER_ROUTER,buffer);

    int msqid_in = getmsgq(MSGQ_BROKER_IN_ROUTER);
    int msqid_out = getmsgq(MSGQ_ROUTER_BROKER_OUT);
    MessageQ msg;

    while (recibirmsgqSinCheckeo(msqid_in,&msg,sizeof(MessageQ),0) != -1){
        //enviarmsgq();
    }

    cerrarLogger(&log);

    return 0;
}

