#include <stdio.h>
#include "sizes.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
char *SendMessage(int sock, char *buffer) {
    int n;

    bzero(buffer, MAX_STRING_SIZE);
    fgets(buffer, MAX_STRING_SIZE - 1, stdin);

    char* msg = buffer;

    n = send(sock, buffer, strlen(buffer), 0);
    if (n < 0) 
        printf("Error: send() Failed.\n");

    return msg;
}
void ReceiveMessage(int socket, char *buffer) {
    int n;
    bzero(buffer, MAX_STRING_SIZE);
    n = recv(socket, buffer, MAX_STRING_SIZE - 1, 0);
    if (n < 0) 
        printf("Error: recv() Failed.\n");
}
