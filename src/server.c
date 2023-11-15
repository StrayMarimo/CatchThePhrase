
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include "sizes.h"
#include "socket_connection.h"


void die_with_error(char *error_msg){
    printf("%s", error_msg);
    exit(-1);
}
void receiveMessage(int sock, char *buffer){
    int n;
    bzero(buffer, 256);
        n = recv(sock, buffer, 255, 0);
    if (n < 0) die_with_error("Error: recv() Failed.");
}
char* sendMessage(int sock, char *buffer){
    int n;

    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    char* msg = buffer;

    n = send(sock, buffer, strlen(buffer), 0);
    if (n < 0) 
        die_with_error("Error: send() Failed.");

    return msg;
}
int main(int argc, char *argv[]){
    int server_sock, client_sock, port_no, n;
    int score = 100;
    char topics[3][MAX_STRING_SIZE] = {"Movies", "Song", "Anime" };
    char phrase[MAX_STRING_SIZE];
    char buffer[256];
    time_t t;

    struct sockaddr_in server_addr, client_addr;
    if (argc < 2) {
        printf("Usage: %s port_no", argv[0]);
        exit(1);
    }

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
    printf("Your topic: %s", sendMessage(client_sock, buffer));

    
    close(client_sock);
    close(server_sock);
    
    return 0; 
}