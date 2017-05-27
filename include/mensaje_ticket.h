#ifndef MENSAJE_TICKET
#define MENSAJE_TICKET

typedef struct Mensaje_ticket {
	long mtype;
	int ticket;
} Mensaje_ticket;

void crearMsgTicket(Mensaje_ticket* msg, long id, int ticket) {
	msg->mtype = id;
	msg->ticket = ticket;
}

void serializeMsgTicket(Mensaje_ticket* msg, char* buffer){
	sprintf(buffer,"%ld %d",msg->mtype,msg->ticket);
}

void deserializeMsgTicket(Mensaje_ticket* msg, char* buffer) {
	char aux[128];
	strcpy(aux,buffer);

	msg->mtype = atol(strtok(aux,SEPARATOR));
	msg->ticket = atoi(strtok(NULL,SEPARATOR));
}

#endif