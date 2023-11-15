#include "socket_connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>

void CreateSocket(int *sock){
    printf("Creating socket...\n");
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock < 0)
        printf("Error: socket() Failed.");
}
struct sockaddr_in *CreateSocketAddress(int port_no) {
    struct sockaddr_in *server_addr = malloc(sizeof(struct sockaddr_in));
    if (server_addr == NULL) {
        perror("Error: malloc failed");
        exit(EXIT_FAILURE);
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(port_no);

    return server_addr;
}
void BindSocket(int *sock, struct sockaddr_in *server_addr) {
    if (bind(*sock, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("Error: bind failed");
    }
    listen(*sock, 5);
    printf("Socket created and listening to port.\n");
}
int HandleNewConnection(int server_sock, struct sockaddr_in *client_addr, socklen_t *client_size) {
    // Accept new connection
    int client_sock = accept(server_sock, (struct sockaddr *)client_addr, client_size);
    if (client_sock < 0) {
        perror("Error: accept() Failed.");
    }

    return client_sock;
}

struct hostent *FindHost(char *hostname){
    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) {
        perror("Error: No such host.");
    }
    return server;
}
void EstablishConnection(int client_sock, struct hostent *server, int port_no) {
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port_no);

    // Connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error: connect() Failed.");
    }
}