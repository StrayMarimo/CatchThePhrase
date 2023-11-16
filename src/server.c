#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include "sizes.h"
#include "socket_connection.h"
#include "socket_communication.h"
#include "common_utils.h"
#include "topics.h"
#include "player.h"
#include <string.h>

void setupSocketConnection(int *server_sock, int *client_sock, int port);
char *setupTopic();

int main(int argc, char *argv[]){
    int server_sock, client_sock, port_no, n;
    struct Player player = CreatePlayer();

    // Setup Connection
    ValidateArgs(argv[0], 2, argc);
    setupSocketConnection(&server_sock, &client_sock, atoi(argv[1]));

    // Setup Topic
    SendMessage(client_sock, setupTopic(), false);
    ReceiveAck(client_sock);

    // Setup Phrases
    SetPhrase(&player, client_sock); 
    SetGuessPhrase(&player, client_sock);

    // Close Connection
    close(client_sock);
    close(server_sock);
    
    return 0; 
}

void setupSocketConnection(int *server_sock, int *client_sock, int port){
    struct sockaddr_in server_addr, client_addr;
    CreateSocket(server_sock);
    server_addr = *CreateSocketAddress(port);
    BindSocket(server_sock, &server_addr);
    socklen_t client_size = sizeof(client_addr);
    *client_sock = HandleNewConnection(*server_sock, &client_addr, &client_size);

    printf("Connected to Player 2.\n");
    usleep(2000);
    system("clear");   
}

char *setupTopic() {
    time_t t;
    srand((unsigned) time(&t));

    char *topic = topics[rand() % MAX_TOPIC];
    printf("Your topic: %s\n", topic);

    return topic;
}