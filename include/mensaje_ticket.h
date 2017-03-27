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



#endif