#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include "sizes.h"
#include "socket_connection.h"
#include "socket_communication.h"
#include "common_utils.h"

int main(int argc, char *argv[]){
    int server_sock, client_sock, port_no, n;
    int score = 100;
    char topics[3][MAX_STRING_SIZE] = {"Movies", "Song", "Anime" };
    char phrase[MAX_STRING_SIZE];
    char buffer[256];
    time_t t;

    struct sockaddr_in server_addr, client_addr;
    ValidateArgs(argv[0], 2, argc);

    CreateSocket(&server_sock);
    server_addr = *CreateSocketAddress(atoi(argv[1]));
    BindSocket(&server_sock, &server_addr);
    socklen_t client_size = sizeof(client_addr);
    client_sock = HandleNewConnection(server_sock, &client_addr, &client_size);

    printf("Connected to Player 2.\n");
    usleep(2000);
    system("clear");
    
    srand((unsigned) time(&t));
    printf("Enter a phrase under topic: %s: ", topics[rand() % 3]);
    printf("Your topic: %s", SendMessage(client_sock, buffer));

    
    close(client_sock);
    close(server_sock);
    
    return 0; 
}