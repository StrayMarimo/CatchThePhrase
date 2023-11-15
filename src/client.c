#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket_connection.h"
#include "socket_communication.h"
#include "sizes.h"

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
    EstablishConnection(client_sock, server, atoi(argv[2]));

    printf("Connected to Player 1.\n");

    usleep(2000);
    system("clear");
    printf("Waiting for Player 1 to choose a topic...\n");

    ReceiveMessage(client_sock, buffer);
    printf("The chosen topic: %s", buffer);

    close(client_sock);
    return 0;
}