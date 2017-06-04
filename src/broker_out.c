#include <stdio.h>

int main(int argc, char** argv){
    if (argc != 2){
        printf("Debería venir 2 argumentos");
        return 1;
    }

    printf("Soy un worker");

    return 0;
}
