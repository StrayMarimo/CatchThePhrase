
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include "socket_connection.h"
#include "sizes.h"

void die_with_error(char *error_msg){
    printf("Error: %s failed", error_msg);
    exit(-1);
}
void receiveMessage(int sock, char *buffer){
    int n;
    bzero(buffer, 256);
    n = recv(sock, buffer, 255, 0);
    if (n < 0) 
        die_with_error("Error: recv() Failed.");
}

char *sendMessage(int sock, char *buffer){
    int n;
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    char *msg = buffer;
    n = send(sock, buffer, strlen(buffer), 0);
    if (n < 0) 
        die_with_error("Error: send() Failed.");
    
    return msg;
}

int main(int argc,  char *argv[]){
    
    int client_sock,  port_no,  n;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[MAX_STRING_SIZE];
    if (argc < 3) {
        printf("Usage: %s hostname port_no",  argv[0]);
        exit(1);
    }

    CreateSocket(&client_sock);
    server = FindHost(argv[1]);
    establishConnection(client_sock, server, atoi(argv[2]));

    printf("Connected to Player 1.\n");

    usleep(2000);
    system("clear");
    printf("Waiting for Player 1 to choose a topic...\n");

    receiveMessage(client_sock, buffer);
    printf("The chosen topic: %s", buffer);
    

    close(client_sock);
    return 0;
}