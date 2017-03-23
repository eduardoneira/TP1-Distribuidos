#ifndef _DEFINES_H_
#define _DEFINES_H_

//Todos los defines para las estructuras de ipc a usar

/**
* 	EstadoHeladeria
*/

#define ID_SHM_ESTADO_HELADERIA		1
#define SEMID_ESTADO_HELADERIA 		2


/**
* MSGS de MSG QUEUE
*/

#define MSG_CLIENTE_AL_CAJERO 		1


// Definiciones de constantes del problema

#define CANTIDAD_GUSTOS 			10
#define TAMANIO_COLA 				20
#define TAMANIO_HELADERIA 			30
#define ABIERTO						1
#define CERRADO						0

/**
* Archivo de log
*/

#define LOG_NAME 					"log.txt"

/*
*  ERRORES
*/

#define ERROR_CREAR_IPC 			-1


#endif