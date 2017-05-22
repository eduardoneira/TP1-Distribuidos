
#ifndef TPS_DISTRIBUIDOS_RPC_WRAPPER_H
#define TPS_DISTRIBUIDOS_RPC_WRAPPER_H

#include "../rpc/rpc.h"

CLIENT* initRPC(const char* host){
    CLIENT *clnt;
    clnt = clnt_create (host, RPCPROG, RPCVERSION, "udp");

    if (clnt == NULL) {
        clnt_pcreateerror (host);
        exit (1);
    }

    return clnt;
}

int getIdRPC(CLIENT* client){
    int  *result_1;
    char *getnumeroid_1_arg;

    result_1 = getnumeroid_1((void*)&getnumeroid_1_arg, client);
    if (result_1 == (int *) NULL) {
        clnt_perror (client, "call failed");
    }

    return *result_1;
}

int getTicketRPC(CLIENT* client) {

    int  *result_2;
    char *getticket_1_arg;

    result_2 = getticket_1((void*)&getticket_1_arg, client);
    if (result_2 == (int *) NULL) {
        clnt_perror (client, "call failed");
    }

    return *result_2;
}

void destruirRPC(CLIENT* client) {
    clnt_destroy(client);
}


#endif //TPS_DISTRIBUIDOS_RPC_WRAPPER_H
