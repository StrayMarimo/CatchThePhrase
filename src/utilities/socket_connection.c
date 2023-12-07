#include "socket_connection.h"
#include "string_values.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include "common_utils.h"

void CreateSocket(int *sock){
    printf("%s\n", CREATING_SOCKET);
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock < 0) {
        DieWithError(SOCKET_ERROR);
    }
}
        
struct sockaddr_in *CreateSocketAddress(int port_no) {
    printf("%s\n", CREATING_SOCKET_ADDRESS);
    struct sockaddr_in *server_addr = malloc(sizeof(struct sockaddr_in));
    if (server_addr == NULL) DieWithError(MALLOC_ERROR);
    
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(port_no);

    return server_addr;
}
void BindSocket(int *sock, struct sockaddr_in *server_addr) {
    printf("%s\n", BINDING_SOCKET);
    if (bind(*sock, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) < 0)
        DieWithError(BIND_ERROR);

    listen(*sock, 5);
    printf("%s\n", LISTENING_SOCKET);
}

int HandleNewConnection(int server_sock, struct sockaddr_in *client_addr, socklen_t *client_size) {
    printf("%s\n", WAITING_FOR_PLAYER);
    int client_sock = accept(server_sock, (struct sockaddr *)client_addr, client_size);
    printf("%s\n", PLAYER_CONNECTED);
    if (client_sock < 0) DieWithError(ACCEPT_ERROR);
    return client_sock;
}

struct hostent *FindHost(char *hostname){
    printf("%s\n", FINDING_HOST);
    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) 
        DieWithError(HOST_ERROR);
    return server;
}
void EstablishConnection(int client_sock, struct hostent *server, int port_no) {
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port_no);

    printf("%s\n", CONNECTING_TO_SERVER);
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        DieWithError(CONNECT_ERROR);
    
    printf("%s\n", CONNECTED_TO_SERVER);
}