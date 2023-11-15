
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_STRING 100


void die_with_error(char *error_msg){
    printf("Error: %s failed", error_msg);
    exit(-1);
}
void receiveMessage(int sock, char *buffer){
    int n;
    bzero(buffer, 256);
    n = recv(sock, buffer, 255, 0);
    if (n < 0) 
        die_with_error("Error: recv() Failed.");
}

char *sendMessage(int sock, char *buffer){
    int n;
   
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    char *msg = buffer;
    n = send(sock, buffer, strlen(buffer), 0);
    if (n < 0) 
        die_with_error("Error: send() Failed.");
    
    return msg;
}


int main(int argc,  char *argv[]){
    
    int client_sock,  port_no,  n;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[MAX_STRING];
    if (argc < 3) {
        printf("Usage: %s hostname port_no",  argv[0]);
        exit(1);
    }

    // Create a socket using TCP
    client_sock = socket(AF_INET,  SOCK_STREAM,  0);
    if (client_sock < 0) 
        die_with_error("Error: socket() Failed.");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        die_with_error("Error: No such host.");
    }

    // Establish a connection to server
    port_no = atoi(argv[2]);
    bzero((char *) &server_addr,  sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,  
         (char *)&server_addr.sin_addr.s_addr, 
         server->h_length);
         
    server_addr.sin_port = htons(port_no);

    if (connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
        die_with_error("Error: connect() Failed.");

    printf("Connected to Player 1.\n");

    usleep(2000);
    system("clear");
    printf("Waiting for Player 1 to choose a topic...\n");

    receiveMessage(client_sock, buffer);
    printf("The chosen topic: %s", buffer);
    

    close(client_sock);
    return 0;
}