# TP4-Distribuidos
TP 4 de Distribuidos : Middleware Distribuido con Broker

# Consigna

Hacer una simulación de una heladería utilizando IPC. Crear dos procesos 'heladeros' y un proceso 'cajero'. El proceso cajero toma pedidos de procesos 'cldiente' que van llegando a la heladeria. Luego atender al cliente y cobrarle el pedido, el cajero pasa los datos del pedido a uno de los heladeros y entrega un vale al cliente. El heladero sirve helado de los gustos elegidos por el cliente y al finalizar se lo entrega al cliente que tenga en posicion el vale correspondiente. Al obtener el helado, el cliente ocupa un lugar en la heladeria hasta que termina de consumir el helado. Al finalizar se retira del lugar.

Suposiciones:
- Los clientes siempre eligen 3 gustos de helado.
- El cajero tarda un tiempo aleatorio en atender a cada cliente.
- El heladero tarda un tiempo aleatorio en servir cada gusto de helado.
- El cliente tarda un tiempo aleatorio en consumir el helado.
- Dos heladeros no pueden acceder al mismo tiempo al mismo gusto de helado.
- La heladeria tiene N lugares disponibles para sentarse. Si se encuentra llena, un cliente puede elegir irse o pedir un helado para llevar. Los clientes no esperan a que haya lugres libres.
- La heladeria tiene M lugares disponibles para que los clientes esperen a ser atendidos. Si no hay mas lugar, los clientes que llegan no se ponen en la cola. Se van.
- Al cerrar el negocio, se le permite a los clientes quedarse dentro del establecimiento hasta finalizar su helado. Luego deben retirarse. No pueden ingresar nuevos clientes.

# Comandos

Acordarse que hay levantar primero al broker. Siempre hay que correr primero el constructor con el parametro adecuado y después el proceso.
Si se quiere modificar el host del BROKER, cambiar en defines.h la constante 'IP_BROKER'.  El trabajo se encuentra bajo el supuesta de que si lanzo un cliente (proceso, no MOM), debe haber al menos un heladero y un cajero. Sino se quedará esperando indefinidamente.


```
cmake .	                            // Crea el make a partir del CMakeLists.txt
make                      	        // Compila el codigo y genera los ejecutables
./constructor [modo]	            // Crea estructuras de IPC y lanza los mom dependiendo del modo. Pueder ser : 'broker', 'cajero', 'heladero' o 'cliente'.
./control [estado_heladeria]        // Abre o cierre la heladeria dependiendo del estado_heladeria. Algun constructor se tuvo que haber creado. [estado_heladeria] puede ser 'abrir' o 'cerrar'.
./broker                            // Lanza el broker luego de haber corrido el ./constructor broker. Para pararlo mandar SIGINT.
./cajero                            // Lanza el cajero luego de haber corrido el ./constructor cajero
./heladero                          // Lanza el heladero luego de haber corrido el ./constructor heladero
./cliente                           // Lanza el cliente luego de haber corrido el ./constructor cliente
./control lanzar [num_clientes]     // Lanza clientes segun el numero que se especifique
./control destruir [modo] [momId]   // Envia msg de destruccion a un cajero o heladero. [modo] puede ser 'heladero' o 'cajero'. [momId] debe ser el momId que utiliza. Fijarse el log
./destructor [modo]  	            // Destruye todas las estructuras de IPC. El modo puede ser "cliente", "cajero", "heladero" o "broker"
```
