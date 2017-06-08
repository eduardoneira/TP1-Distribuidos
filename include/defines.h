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

//BROKER
#define MSGQ_BROKER_IN_ROUTER               11
#define MSGQ_ROUTER_BROKER_OUT              12

//NUEVAS COLAS
#define MSGQ_RECIBIR_CLIENTE                13
#define MSGQ_RECIBIR_CAJERO                 14
#define MSGQ_RECIBIR_HELADERO               15
#define MSGQ_POR_PID_CLIENTE                16
#define MSGQ_POR_TICKET                     17
#define MSGQ_POR_MOMID_CAJERO               18
#define MSGQ_POR_MOMID_CLIENTE              19
#define MSGQ_POR_MOMID_HELADERO             20
#define MSGQ_POR_PID_CAJERO	                21
#define MSGQ_POR_PID_HELADERO               22

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
* Archivos
*/

#define LOG_NAME 					"log.txt"
#define IPS                         "ips.txt"

/**
 * NOMBRES PARA LOGGEAR
 * */

#define MOM                 "MOM\t\t"
#define CLIENTE_NAME 	    "CLIENTE\t"
#define CAJERO_NAME	        "CAJERO\t\t"
#define MANAGER_NAME        "MANAGER\t"
#define HELADERO_NAME       "HELADERO\t"
#define CONSTRUCTOR         "CONSTRUCTOR"
#define DESTRUCTOR          "DESTRUCTOR"
#define MOM_REGISTER        "MOM_REGISTER"
#define BROKER_NAME		    "BROKER\t"
#define BROKER_ROUTER_NAME	"BROKER_ROUTER"
#define BROKER_IN_NAME	    "BROKER_IN\t"
#define BROKER_OUT_NAME	    "BROKER_OUT\t"

/**
 * NOMBRES DE MODOS
 * */

#define CAJERO 		"cajero"
#define HELADERO	"heladero"
#define CLIENTE		"cliente"
#define BROKER      "broker"
#define RPC         "rpc"
#define ALL			"all"

/**
 * CANTIDAD PROCESOS
 */

#define CANT_HELADEROS              2
#define CANT_CAJEROS                1
#define CANT_TIPOS_PROCESOS  4

/**
*  ERRORES
*/

#define ERROR_CREAR_IPC 			-1
#define ERROR_DESTRUIR_IPC			-2

/**
 * HOSTS
 * */
#define LOCALHOST   "127.0.0.1"
#define IP_BROKER   "127.0.0.1"
#define IP_RPC      "127.0.0.1"
#define PORT_BROKER 30000


/**
 * SOCKET
 * */
#define MAX_READ_SOCKET     1024
#define SOCKET_ACTIVO       "activo"
#define SOCKET_PASIVO       "pasivo"

/**
 * MSGS COSAS
 * */
#define SEPARATOR                       "-"
#define MSG_BROKER_REGISTER             1
#define MSG_BROKER_TICKET               2
#define MSG_BROKER_HELADO               3
#define MSG_BROKER_PEDIDO               4
#define MSG_BROKER_OCUPAR_HELADO        5
#define MSG_BROKER_DESOCUPAR_HELADO     6
#define MSG_BROKER_DESREGISTRARSE       7
#define MSG_BROKER_CERRAR_HELADERIA     8
#define MSG_BROKER_ABRIR_HELADERIA      9
#define MSG_BROKER_PUEDO_ENTRAR         10
#define MSG_BROKER_HAY_LUGAR_COLA       11
#define MSG_BROKER_SALIR_COLA           12
#define MSG_BROKER_HAY_LUGAR_SENTARSE   13
#define MSG_BROKER_CERRAR               14
#define SIZE_TYPE_MSG                   5
#define SIZE_PAYLOAD_MSG                64

#endif