#include "int_values.h"
#include <stdbool.h>

#ifndef SOCKET_COMMUNICATION_H
#define SOCKET_COMMUNICATION_H

void SendMessage(int socket, const char *message);
void ReceiveMessage(int socket, char *message);
void SendAck(int socket);
void ReceiveAck(int socket);
#endif