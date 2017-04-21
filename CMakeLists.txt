cmake_minimum_required(VERSION 3.6)
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
        include/fifo.h
        include/fifo_escritura.h
        include/fifo_lectura.h
        include/lockfile.h
        include/logger.h
        include/memoria_compartida.h
        include/mensaje_gustos.h
        include/mensaje_helado.h
        include/mensaje_ticket.h
        include/msg_queue.h
        include/semaforo.h
        include/util.h
        include/
)

#COMMON SOURCE FILES
set(SOURCE_FILES
		src/SignalHandler.c
		src/SIG_Trap.c)

#CAJERO
set(CAJERO_FILES
        src/cajero.c
        include/ICajeroMOM.h
        include/cajero_handler.h)

#HELADERO
set(HELADERO_FILES
        src/heladero.c
        include/IHeladeroMOM.h
        include/heladero_handler.h)

add_executable(constructor_heladeria ${HEADER_FILES} src/constructor_heladeria.c)
add_executable(destructor_heladeria ${HEADER_FILES} src/destructor_heladeria.c)
add_executable(control ${HEADER_FILES} src/control.c)
add_executable(heladero ${HEADER_FILES} ${HELADERO_FILES})
add_executable(cliente ${HEADER_FILES} src/cliente.c)
add_executable(cajero ${HEADER_FILES} ${CAJERO_FILES})

add_executable(signalHandlerTest ${HEADER_FILES} ${SOURCE_FILES} src/SignalHandler.cpp)