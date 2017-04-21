#ifndef SIG_TRAP_H
#define SIG_TRAP_H

#include <signal.h>
#include <assert.h>

typedef struct SIG_Trap
{
    sig_atomic_t signal_received;
    int my_signal;
} SIG_Trap;

void SIG_TrapInit(SIG_Trap* sigtrap, int mysignal);
int SIG_TrapHandleSignal(SIG_Trap* sigtrap, int signum);
sig_atomic_t SIG_TrapSignalWasReceived(SIG_Trap* sigtrap);
void SIG_TrapReset(SIG_Trap* sigtrap);

#endif // SIG_TRAP_H
