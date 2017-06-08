#include <stdio.h>
#include "../include/broker_router.h"

bool route(Router_handler* handler,MessageQ* msg,Logger* log) {

    int type = atoi(msg->type);

    switch (type) {
        case MSG_BROKER_REGISTER:
            return registrarMOM(handler,msg);
        case MSG_BROKER_TICKET:
            return enviarTicket(handler,msg);
        case MSG_BROKER_HELADO:
            return enviarHelado(handler,msg);
        case MSG_BROKER_PEDIDO:
            return enviarPedido(handler,msg);
        case MSG_BROKER_OCUPAR_HELADO:
            return ocuparHelado(handler,msg);
        case MSG_BROKER_DESOCUPAR_HELADO:
            return desocuparHelado(handler,msg);
        case MSG_BROKER_DESREGISTRARSE:
            return desregistrarseMOM(handler,msg);
        case MSG_BROKER_CERRAR_HELADERIA:
            return cambiarEstadoHeladeria(handler,CERRADO);
        case MSG_BROKER_ABRIR_HELADERIA:
            return cambiarEstadoHeladeria(handler,ABIERTO);
        case MSG_BROKER_PUEDO_ENTRAR:
            return clientePuedeEntrar(handler,msg);
        case MSG_BROKER_HAY_LUGAR_COLA:
            return hay_lugar_cola(handler,msg);
        case MSG_BROKER_SALIR_COLA:
            return salir_cola(handler);
        case MSG_BROKER_HAY_LUGAR_SENTARSE:
            return hay_lugar_heladeria(handler,msg);
        case MSG_BROKER_CERRAR:
            return true;
        default:
            return false;
    }
}

int main(int argc, char** argv){
    //Log inicial
    Logger log = crearLogger();
    pid_t pid = getpid();
    char buffer[64];

    escribirLog(&log,DEBUG,pid,BROKER_ROUTER_NAME,"Soy un router broker");

    int msqid_in = getmsgq(MSGQ_BROKER_IN_ROUTER);
    int msqid_out = getmsgq(MSGQ_ROUTER_BROKER_OUT);
    MessageQ msg;

    Router_handler handler = crearRouterHandler();

    while (recibirmsgqSinCheckeo(msqid_in,&msg,sizeof(MessageQ),0) != -1){
        sprintf(buffer,"Recibi un msg por in, hora de router. TYPE:%s, PAYLOAD: %s",msg.type,msg.payload);
        escribirLog(&log,DEBUG,pid,BROKER_ROUTER_NAME,buffer);
        if (route(&handler,&msg,&log)) {
            sprintf(buffer,"Voy a enviar lo siguiente. TYPE:%s, PAYLOAD: %s",msg.type,msg.payload);
            escribirLog(&log,DEBUG,pid,BROKER_ROUTER_NAME,buffer);
            enviarmsgq(msqid_out,&msg,sizeof(MessageQ));
        }
    }

    destruirRouterHandler(&handler);
    escribirLog(&log,DEBUG,pid,BROKER_ROUTER_NAME,"Cerraron la cola, me fui");
    cerrarLogger(&log);

    return 0;
}

