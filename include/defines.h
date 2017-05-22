#ifndef _DEFINES_H_
#define _DEFINES_H_

//Todos los defines para las estructuras de ipc a usar

/**
* 	EstadoHeladeria
*/

#define ID_SHM_ESTADO_HELADERIA		1
#define SEMID_ESTADO_HELADERIA 		1>>8


/**
* ID de MSG QUEUE
*/

//CAJERO
#define MSGQ_PASAMANOS_CAJERO_MOM_PEDIDO    1
#define MSGQ_PASAMANOS_MOM_CAJERO_PEDIDO    2
#define MSGQ_PASAMANOS_CAJERO_MOM_TICKET    3

//HELADERO
#define MSGQ_PASAMANOS_MOM_HELADERO_PEDIDO  4
#define MSGQ_PASAMANOS_HELADERO_MOM_HELADO  5

//CLIENTE
#define MSGQ_PASAMANOS_CLIENTE_MOM_PEDIDO   6
#define MSGQ_PASAMANOS_MOM_CLIENTE_TICKET   7
#define MSGQ_PASAMANOS_MOM_CLIENTE_HELADO   8

//TODOS
#define MSGQ_REGISTER_MOM                   9
#define MSGQ_DESTRUCTOR                     10


/**
* SEMAFOROS HELADERIA
*/

#define PRIMER_ID_GUSTO_HELADO		1

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

/**
 * NOMBRES PARA LOGGEAR
 * */

#define MOM             "MOM\t\t"
#define CLIENTE_NAME 	"CLIENTE\t"
#define CAJERO_NAME	    "CAJERO\t\t"
#define MANAGER_NAME    "MANAGER\t"
#define HELADERO_NAME   "HELADERO\t"
#define CONSTRUCTOR     "CONSTRUCTOR"
#define DESTRUCTOR      "DESTRUCTOR"
#define MOM_REGISTER    "MOM_REGISTER"

/**
 * NOMBRES
 * */

#define CAJERO 		"cajero"
#define HELADERO	"heladero"
#define CLIENTE		"cliente"
#define ALL			"all"

/**
 * CANTIDAD PROCESOS
 */

#define CANT_HELADEROS  2
#define CANT_CAJEROS    1

/**
*  ERRORES
*/

#define ERROR_CREAR_IPC 			-1
#define ERROR_DESTRUIR_IPC			-2

#endif