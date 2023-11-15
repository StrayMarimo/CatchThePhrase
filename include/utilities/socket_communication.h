#ifndef SOCKET_COMMUNICATION_H
#define SOCKET_COMMUNICATION_H

char *SendMessage(int socket, char *message);
void *ReceiveMessage(int socket, char *message);

#endif