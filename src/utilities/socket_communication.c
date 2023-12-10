#include <stdio.h>
#include "int_values.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common_utils.h"
#include "socket_communication.h"

void SendMessage(int socket, const char *message) {
    size_t len = strlen(message);
    send(socket, &len, sizeof(size_t), 0); 
    send(socket, message, len, 0);  
}

void ReceiveMessage(int socket, char *message) {
    size_t len;
    recv(socket, &len, sizeof(size_t), 0); 
    recv(socket, message, len, 0);          
    message[len] = '\0';           
}

void SendAck(int socket) {
    char *message = "ACK";
    size_t len = strlen(message);
    send(socket, &len, sizeof(size_t), 0); 
    send(socket, message, len, 0);  
}

void ReceiveAck(int socket) {
    char *message;
    size_t len;
    recv(socket, &len, sizeof(size_t), 0); 
    recv(socket, message, len, 0);
    message[len] = '\0';  
}