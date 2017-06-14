
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

void getHostRPC(char* ipRPC,const char* pname){
    char nombre[20];
    char ip[20];
    char ipHost[20];

    FILE* fd = fopen(IPS,"r");

    if (fd == NULL) {
        perror("No se encontro el archivo de ips.txt. Asegurarse que este en la misma carpeta");
    }

    while (!feof(fd)) {
        fscanf(fd,"%s %s\n",nombre,ip);
        if (strcmp(pname,nombre) == 0){
            strcpy(ipHost,ip);
        }

        if(strcmp(nombre,RPC) == 0){
            strcpy(ipRPC,ip);
        }
    }

    fclose(fd);
    if (strcmp(ipRPC,ipHost) == 0){
        strcpy(ipRPC,LOCALHOST);
    }

}

void destruirRPC(CLIENT* client) {
    clnt_destroy(client);
}


#endif //TPS_DISTRIBUIDOS_RPC_WRAPPER_H
