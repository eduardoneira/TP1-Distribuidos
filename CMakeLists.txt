cmake_minimum_required(VERSION 3.2)
project(TPS_Distribuidos)

#SETEO COMPILADORES A G++
SET(CMAKE_C_COMPILER /usr/bin/g++)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)

#COMMON HEADERS
set(HEADER_FILES
        include/constructor.h
        include/defines.h
        include/destructor.h
        include/estado_heladeria.h
        include/lockfile.h
        include/logger.h
        include/memoria_compartida.h
        include/mensaje_gustos.h
        include/mensaje_helado.h
        include/mensaje_ticket.h
        include/msg_queue.h
        include/semaforo.h
        include/util.h
		include/mensaje_registro.h include/socket.h)

#COMMON SOURCE FILES
set(SOURCE_FILES
		src/SignalHandler.c
		src/SIG_Trap.c)

#CAJERO
set(CAJERO_FILES
        src/cajero.c
        include/ICajeroMOM.h
        include/cajero_handler.h)

#RPC
set(RPC_FILES
		rpc/rpc_clnt.c
		rpc/rpc.h
		include/rpc_wrapper.h)

#HELADERO
set(HELADERO_FILES
        src/heladero.c
        include/IHeladeroMOM.h
        include/heladero_handler.h)

#CLIENTE
set(CLIENTE_FILES
        src/cliente.c
        include/IClienteMOM.h
        include/cliente_handler.h)

#MOM_SERVER
set(MOM_SERVER
        src/MOM_server.c
        include/MOM_server.h)

#MOM_REGISTER
set(MOM_REGISTER
		src/MOM_register.c)

add_executable(constructor_heladeria ${HEADER_FILES} src/constructor_heladeria.c)
add_executable(destructor_heladeria ${HEADER_FILES} src/destructor_heladeria.c)
add_executable(control ${HEADER_FILES} src/control.c)
add_executable(heladero ${HEADER_FILES} ${HELADERO_FILES})
add_executable(cliente ${HEADER_FILES} ${CLIENTE_FILES})
add_executable(cajero ${HEADER_FILES} ${CAJERO_FILES} ${RPC_FILES})
add_executable(MOM_server ${HEADER_FILES} ${SOURCE_FILES} ${MOM_SERVER})
add_executable(MOM_register ${HEADER_FILES} ${SOURCE_FILES} ${RPC_FILES} ${MOM_REGISTER})
add_executable(signalHandlerTest ${HEADER_FILES} ${SOURCE_FILES} tests/SignalHandlerTestMain.c)