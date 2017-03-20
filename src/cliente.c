#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



int main(int argc, char** argv) {
	
	printf("Soy un cliente. Mi pid es %d\n",getpid());

	return 0;
}