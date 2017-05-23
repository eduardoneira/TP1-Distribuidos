
#ifndef TPS_DISTRIBUIDOS_SOCKET_H_H
#define TPS_DISTRIBUIDOS_SOCKET_H_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

int abrir_socket_activo(char* ip, int port){
    struct sockaddr_in serv_addr;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        perror("Fallo llamada a socket\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        perror("Fallo el connect del socket\n");
        return -1;
    }

    return fd;
}

int abrir_socket_pasivo(char* ip, int port){
    struct sockaddr_in serv_addr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd == -1){
        perror("Fallo llamada a socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        close(listenfd);
        printf("Error al bindear a %s:%d\n",ip,port);
        return -1;
    }

    if (listen(listenfd, 10) == -1){
        close(listenfd);
        perror("Error al hacer listen\n");
        return -1;
    }

    int nfd = accept(listenfd,(struct sockaddr*) NULL,NULL);

    if (nfd == -1) {
        close(listenfd);
        perror("Error al hacer accept\n");
        return -1;
    }

    close(listenfd);
    return nfd;
}

int escribir_socket(int fd, void* data, int size_data){
    return write(fd,data,size_data);
}

int leer_socket(int fd, void* data, int size_data){
    int leido = 0;
    int leidoActual = 0;
    char buffer[1024];

    while (leido != size_data){
        leidoActual = read(fd, buffer, size_data - leido);

        if (leidoActual == -1){
            perror("Error al leer\n");
            return -1;
        }

        if (leidoActual == 0){
            printf("Se cerro la conexion por alguna razon\n");
            return -1;
        }

        memcpy((unsigned char*)data+leido,buffer,leidoActual);
        leido+=leidoActual;
    }

    return leido;
}

int cerrar_socket(int fd){
    return close(fd);
}

#endif //TPS_DISTRIBUIDOS_SOCKET_H_H
