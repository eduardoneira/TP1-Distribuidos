#ifndef MENSAJE_HELADO
#define MENSAJE_HELADO

#define HELADO_SIZE 255

typedef struct Mensaje_helado {
	long mtype;
	char helado[HELADO_SIZE];
}	Mensaje_helado;

void crear(Mensaje_helado* msg, long id, char* helado){
	msg->mtype = id;
	strcpy(msg->helado,helado);
}

#endif