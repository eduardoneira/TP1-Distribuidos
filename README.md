# TP1-Distribuidos
TP 1 de Distribuidos : Repaso de Concu

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

make                      // compila el codigo y genera los ejecutables
./constructor_heladeria   // lanza un proceso cajero y dos procesos heladeros con forks
./control 5               // lanza 5 (o el numero ingresado) procesos cliente que actuan segun la consigna
./control                 // si no recibe un numero de clientes como argumento, control cambia el estado de la heladeria a cerrada (pero no destruye ningun mecanismo de IPC)
./destructor_heladeria    // envia mensajes a los procesos que se bloquean escuchando mensajes para que se cierren, y tambien destruye las estructuras de IPC creadas. No hace nada si la heladeria no esta cerrada o todavia hay clientes comiendo
