#include <stdio.h>
#include "../include/broker_router.h"

bool route(Router_handler* handler,MessageQ* msg,Logger* log){
    return true;
}

int main(int argc, char** argv){
    //Log inicial
    Logger log = crearLogger();
    pid_t pid = getpid();

    escribirLog(&log,DEBUG,pid,BROKER_ROUTER_NAME,"Soy un router broker");

    int msqid_in = getmsgq(MSGQ_BROKER_IN_ROUTER);
    int msqid_out = getmsgq(MSGQ_ROUTER_BROKER_OUT);
    MessageQ msg;

    Router_handler handler = crearRouterHandler();

    while (recibirmsgqSinCheckeo(msqid_in,&msg,sizeof(MessageQ),0) != -1){
        escribirLog(&log,DEBUG,pid,BROKER_ROUTER_NAME,"Recibi un msg por in, time to route");
        if (route(&handler,&msg,&log)) {
            enviarmsgq(msqid_out,&msg,sizeof(MessageQ));
        }
    }

    escribirLog(&log,DEBUG,pid,BROKER_ROUTER_NAME,"Cerraron la cola, me fui");
    cerrarLogger(&log);

    return 0;
}

