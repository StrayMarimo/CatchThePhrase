#include "socket_connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <ncurses.h>
#include "common_utils.h"

void CreateSocket(int *sock){
    PrintSysMessage(0, "Creating socket...");
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock < 0) {
        napms(500);
        PrintSysMessage(1, "Creating socket...");
        DieWithError(0, "Error: socket() Failed.");
}
    }
        
struct sockaddr_in *CreateSocketAddress(int port_no) {
    struct sockaddr_in *server_addr = malloc(sizeof(struct sockaddr_in));
    if (server_addr == NULL) {
        napms(500);
        PrintSysMessage(1, "Creating socket...");
        DieWithError(0, "Error: malloc failed");
    }
    

    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(port_no);

    return server_addr;
}
void BindSocket(int *sock, struct sockaddr_in *server_addr) {
    if (bind(*sock, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) < 0) {
        napms(500);
        PrintSysMessage(1, "Creating socket...");
        DieWithError(0, "Error: bind failed");
    }
        
    
    listen(*sock, 5);
    napms(500);
    PrintSysMessage(1, "Creating socket...");
    PrintSysMessage(0, "Socket created and listening to port.");
}
int HandleNewConnection(int server_sock, struct sockaddr_in *client_addr, socklen_t *client_size) {
    int client_sock = accept(server_sock, (struct sockaddr *)client_addr, client_size);
    if (client_sock < 0) {
        napms(500);
        PrintSysMessage(2, "Creating socket...");
        PrintSysMessage(1, "Socket created and listening to port.");
        DieWithError(0, "Error: accept() Failed.");
    }
       
    return client_sock;
}

struct hostent *FindHost(char *hostname){
    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) {
        PrintSysMessage(1, "Creating socket...");
        DieWithError(0, "Error: no such host.");
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
        PrintSysMessage(1, "Creating socket...");
        DieWithError(0, "Error: connect() Failed.");
    }

    napms(500);
    PrintSysMessage(1, "Creating socket...");
    PrintSysMessage(0, "Connected to Player 1.");
}