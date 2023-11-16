#include <stdbool.h>
#ifndef SOCKET_COMMUNICATION_H
#define SOCKET_COMMUNICATION_H

void SendMessage(int socket, char *message, bool fromInput);
void ReceiveMessage(int socket, char *message, bool fromInput);
void SendAck(int socket);
void ReceiveAck(int socket);
#endif