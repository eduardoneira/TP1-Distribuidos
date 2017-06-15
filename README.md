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

# Ejemplo de ejecucion

```
cmake .
make
./constructor broker
./broker
./constructor cajero
./constructor heladero
./constructor cliente
./control abrir

// levato los cajeros que quiera
./cajero

// levanto los heladeros que quiera
./heladero
./heladero
./heladero

// levanto 5 clientes
./control lanzar 5

// cierro la heladeria
./control cerrar
```

# Notas

En la carpeta actual se encuentra una ilustración de como funciona el broker. Cuando un nuevo MOM se conecta en la computado del broker se forkean un proceso IN y otro OUT que van a interacturas con dicho MOM y pasar por broker router.
A su vez en la computadora donde se uso el constructor se forkea de ese MOM un proceso que va a escribir el socket, y el padre se va a escucha a leer. Los mensajes se encuentran serializados y se utilizan los primeros 5 bytes para marcan el tipo de mensaje. En base a esto, el router decide como handlear el msg.
En cuanto al balanceo de carga, esta hecho como round-robin, es decir, se reparten los pedidos de manera equitativa tanto para heladeros como cajeros.
Al ser un broker centralizado, ya no se usan ni semaforos ni una shared memory, todas son variables del broker router. Para resolver el problema de ocupar helados y no bloquearse, se siguió el esquema explicado en clase. Se guarda un recordatorio de que momId pidió el helado y una vez que se libera se lo notifica routeandolo.
La tabla de routeo consta de momId - ticket - mtype. El mtype indica con que mtype hay q mandar en la cola de OUT para que se lo envie a la compu de dicho momId. A su vez, cuando se quiere pasar el helado al cliente, el routeo esta hecho por ticket en vez de momId por que quedo de las primeras entregas. Esto no es problema ya que el ticket es único como el momId e identifica a un cliente, sola hace falta saber en que computadora se encuentra.
