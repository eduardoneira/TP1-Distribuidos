#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#include "../include/destructor.h"

int main(int argc, char** argv) {

	return destruirIPC(argc);

}