
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


#define MAX_STRING 100

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
    socklen_t client_size;
    int score = 100;
    char topics[3][MAX_STRING] = {"Movies", "Song", "Anime" };
    char phrase[MAX_STRING];
    char buffer[256];
    time_t t;

    struct sockaddr_in server_addr, client_addr;
    if (argc < 2) {
        printf("Usage: %s port_no", argv[0]);
        exit(1);
    }
    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) 
       die_with_error("Error: socket() Failed.");
       
    // Bind socket to a port
    bzero((char *) &server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);
    server_addr.sin_family = AF_INET; // Internet address 
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(port_no); // Local port
    
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
       die_with_error("Error: bind() Failed.");
    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);

    printf("Waiting for connection...\n");
    // Accept new connection
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);
    if (client_sock < 0) 
        die_with_error("Error: accept() Failed.");  
    
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