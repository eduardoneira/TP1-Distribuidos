
#ifndef TPS_DISTRIBUIDOS_MESSAGE_WRAPPER_H
#define TPS_DISTRIBUIDOS_MESSAGE_WRAPPER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

typedef struct Message {
    char type[SIZE_TYPE_MSG];
    char payload[SIZE_PAYLOAD_MSG];
} Message;

typedef struct MessageQ {
    long mtype;
    char type[SIZE_TYPE_MSG];
    char payload[SIZE_PAYLOAD_MSG];
} MessageQ;

void crearMessageQ(Message* msg, MessageQ* msgq, long mtype){
    msgq->mtype = mtype;
    strcpy(msgq->type,msg->type);
    strcpy(msgq->payload,msg->payload);
}

void crearMessage(Message* msg, MessageQ* msgq){
    strcpy(msg->type,msgq->type);
    strcpy(msg->payload,msgq->payload);
}

bool esMsgRegistrarse(Message msg){
    return (atoi(msg.type) == MSG_BROKER_REGISTER);
}

bool esMsgDesregistrarse(Message msg){
    return (atoi(msg.type) == MSG_BROKER_DESREGISTRARSE);
}

#endif //TPS_DISTRIBUIDOS_MESSAGE_WRAPPER_H
