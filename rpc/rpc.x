program RPCPROG {
        version RPCVERSION {
                int getNumeroId() = 1;
                int getTicket() = 2;
        } = 1;
} = 0x20000001;