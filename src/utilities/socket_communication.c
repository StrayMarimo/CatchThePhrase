#include <stdio.h>
#include "sizes.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include "common_utils.h"
void SendMessage(int sock, char *buffer, bool fromInput) {
    int n;
    if (fromInput) {
        bzero(buffer, MAX_STRING_SIZE);
        getnstr(buffer, MAX_STRING_SIZE - 1);
    }

    n = send(sock, buffer, strlen(buffer), 0);
    if (n < 0) 
        DieWithError(0, "Error: send() Failed.\n");
}

void ReceiveMessage(int socket, char* buffer, bool fromInput) {
    int n;
    if (fromInput)
        bzero(buffer, MAX_STRING_SIZE);
    
    n = recv(socket, buffer, MAX_STRING_SIZE - 1, 0);
    if (n < 0) 
        DieWithError(0, "Error: recv() Failed.\n");
}

void SendAck(int socket) {
    send(socket, "ACK", 3, 0);
}

void ReceiveAck(int socket) {
    char buffer[MAX_STRING_SIZE];
    recv(socket, buffer, MAX_STRING_SIZE - 1, 0);
}