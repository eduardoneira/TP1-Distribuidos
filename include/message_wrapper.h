
#ifndef TPS_DISTRIBUIDOS_MESSAGE_WRAPPER_H
#define TPS_DISTRIBUIDOS_MESSAGE_WRAPPER_H

#include "defines.h"


typedef struct Message {
    char type[SIZE_TYPE_MSG];
    char payload[SIZE_PAYLOAD_MSG];
}Message;

#endif //TPS_DISTRIBUIDOS_MESSAGE_WRAPPER_H
