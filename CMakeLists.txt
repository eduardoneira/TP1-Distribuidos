cmake_minimum_required(VERSION 3.6)
project(TP1_Distribuidos)

SET(CMAKE_C_COMPILER /usr/bin/g++)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)

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
)

add_executable(constructor_heladeria ${HEADER_FILES} src/constructor_heladeria.c)
add_executable(destructor_heladeria ${HEADER_FILES} src/destructor_heladeria.c)
add_executable(control ${HEADER_FILES} src/control.c)
add_executable(heladero ${HEADER_FILES} src/heladero.c)
add_executable(cliente ${HEADER_FILES} src/cliente.c)
add_executable(cajero ${HEADER_FILES} src/cajero.c)