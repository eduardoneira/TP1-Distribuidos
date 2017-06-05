#ifndef MENSAJE_HELADO
#define MENSAJE_HELADO

#define HELADO_SIZE 32

typedef struct Mensaje_helado {
	long mtype;
	char helado[HELADO_SIZE];
}	Mensaje_helado;

void crearMensajeHelado(Mensaje_helado* msg, long id, char* helado){
	msg->mtype = id;
	strcpy(msg->helado,helado);
}

void serializeMsgHelado(Mensaje_helado* msg, char* buffer){
	sprintf(buffer,"%ld-%s",msg->mtype,msg->helado);
}

void deserializeMsgHelado(Mensaje_helado* msg, char* buffer) {
	char aux[128];
	strcpy(aux,buffer);

	msg->mtype = atol(strtok(aux,SEPARATOR));
	strcpy(msg->helado,strtok(NULL,SEPARATOR));
}

#endif