#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <netdb.h>
#include "common_utils.h"
#include "raylib.h"
#include "string_values.h"

#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H

extern int framesCounter;
void CreateSocket(int *sock);

// Server Side
struct sockaddr_in *CreateSocketAddress(int port_no);
void BindSocket(int *sock, struct sockaddr_in *server_addr);
int HandleNewConnection(int server_sock, struct sockaddr_in *client_addr, socklen_t *client_size);

// Client Side
struct hostent *FindHost(char *hostname);
void EstablishConnection(int client_sock, struct hostent *server, int port_no);
#endif