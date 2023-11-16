#include <stdio.h>
#include "sizes.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void SendMessage(int sock, char *buffer, bool fromInput) {
    int n;
    if (fromInput) {
        bzero(buffer, MAX_STRING_SIZE);
        fgets(buffer, MAX_STRING_SIZE - 1, stdin);
    }

    n = send(sock, buffer, strlen(buffer), 0);
    if (n < 0) 
        printf("Error: send() Failed.\n");
}

void ReceiveMessage(int socket, char* buffer, bool fromInput) {
    int n;
    if (fromInput)
        bzero(buffer, MAX_STRING_SIZE);
    
    n = recv(socket, buffer, MAX_STRING_SIZE - 1, 0);
    if (n < 0) 
        printf("Error: recv() Failed.\n");
}

void SendAck(int socket) {
    send(socket, "ACK", 3, 0);
}

void ReceiveAck(int socket) {
    char buffer[3];
    recv(socket, buffer, 3, 0);
}