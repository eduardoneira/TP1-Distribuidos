#include "SIG_Trap.h"

void SIG_TrapInit(SIG_Trap* sigtrap, int mysignal)
{
	sigtrap->signal_received = 0;
	sigtrap->my_signal = mysignal;
}

int SIG_TrapHandleSignal(SIG_Trap* sigtrap, int signum)
{
	assert ( signum == sigtrap->my_signal );
    sigtrap->signal_received = 1;
    return 0;
}

sig_atomic_t SIG_TrapSignalWasReceived(SIG_Trap* sigtrap)
{
	return sigtrap->signal_received;
}

void SIG_TrapReset(SIG_Trap* sigtrap)
{
	sigtrap->signal_received = 0;
}
