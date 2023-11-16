#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket_connection.h"
#include "socket_communication.h"
#include "sizes.h"
#include "common_utils.h"
#include "player.h"
#include <string.h>


int main(int argc,  char *argv[]){
    int client_sock;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[MAX_STRING_SIZE];

    ValidateArgs(argv[0], 3, argc);

    struct Player player = CreatePlayer();
    CreateSocket(&client_sock);
    server = FindHost(argv[1]);
    EstablishConnection(client_sock, server, atoi(argv[2]));

    printf("Connected to Player 1.\n");
    usleep(2000);
    system("clear");

    ReceiveMessage(client_sock, buffer, false);
    printf("The chosen topic: %s\n", buffer);
    SendAck(client_sock);

    printf("Waiting for Player 1 to choose a topic...\n");
    SetGuessPhrase(&player, client_sock);

    SetPhrase(&player, client_sock);
    

    close(client_sock);
    return 0;
}


