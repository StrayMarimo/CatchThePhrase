#include <stdio.h>
#include "int_values.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common_utils.h"
void SendMessage(int sock, char *buffer) {
    int n;

    n = send(sock, buffer, strlen(buffer), 0);
    if (n < 0) 
        DieWithError("Error: send() Failed.\n");
}

void ReceiveMessage(int socket, char* buffer) {
    int n;
    
    n = recv(socket, buffer, MAX_STRING_SIZE - 1, 0);
    if (n < 0) 
        DieWithError("Error: recv() Failed.\n");
}

void SendAck(int socket) {
    send(socket, "ACK", 3, 0);
}

void ReceiveAck(int socket) {
    char buffer[MAX_STRING_SIZE];
    recv(socket, buffer, MAX_STRING_SIZE - 1, 0);
}