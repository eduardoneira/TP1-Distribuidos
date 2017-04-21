#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <signal.h>
#include <stdio.h>
#include <memory.h>

#include "SIG_Trap.h"

SIG_Trap* SignalHandlerRegisterHandler ( int signum, SIG_Trap* eh );
int SignalHandlerRemoveHandler ( int signum );

#endif /* SIGNALHANDLER_H_ */
