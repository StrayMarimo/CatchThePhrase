#include <sys/socket.h>
#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H

void CreateSocket(int *sock);

// Server Side
struct sockaddr_in *CreateSocketAddress(int port_no);
void BindSocket(int *sock, struct sockaddr_in *server_addr);
int HandleNewConnection(int server_sock, struct sockaddr_in *client_addr, socklen_t *client_size);

// Client Side
struct hostent *FindHost(char *hostname);
void establishConnection(int client_sock, struct hostent *server, int port_no);
#endif