# deprecated, should use cmake to compile

all:
	g++ -g src/constructor_heladeria.c -o constructor_heladeria
	g++ -g src/destructor_heladeria.c -o destructor_heladeria
	g++ -g src/cliente.c -o cliente
	g++ -g src/cajero.c -o cajero
	g++ -g src/heladero.c -o heladero
	g++ -g src/control.c -o control