#include <stdbool.h>
#include "../include/SignalHandler.h"
#include "../include/logger.h"
#include "../include/msg_queue.h"
#include "../include/mensaje_registro.h"


//TODO: RPC
int main(int argc, char** argv){
    Logger log = crearLogger();
    pid_t pid = getpid();
    escribirLog(&log,TRACE,pid,MOM_REGISTER,"Hola soy el MOM para registrarse");

    SIG_Trap SIGINT_trap;
    SIG_TrapInit(&SIGINT_trap, SIGINT);

    Mensaje_registro msg;
    int msq_id = getmsgq(MSGQ_REGISTER_MOM);
    int id = 1;

    SignalHandlerRegisterHandler(SIGINT, &SIGINT_trap);

    bool cerrar = false;

    while(SIG_TrapSignalWasReceived(&SIGINT_trap) == 0 && !cerrar){
        recibirmsgq(msq_id,&msg,sizeof(Mensaje_registro),MENSAJE_A_REGISTRO);
        if (!esMsgCerrar(msg)) {
            msg.mtype = msg.id;
            msg.id = id;
            enviarmsgq(msq_id,&msg,sizeof(Mensaje_registro));
            id++;
        } else {
            cerrar = true;
        }
    }

    cerrarLogger(&log);

    return 0;
}