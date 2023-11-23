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
#include <ncurses.h>

void setupSocketConnection(int *client_sock, int port, char *host);

int main(int argc,  char *argv[]){
    int client_sock;
    struct Player player = CreatePlayer();
    char buffer[MAX_STRING_SIZE];
    bool isGuessing = false;

    initscr();
    raw();
    keypad(stdscr, TRUE);

    // Setup Connection
    ValidateArgs(argv[0], 3, argc);
    setupSocketConnection(&client_sock, atoi(argv[2]), argv[1]);

    // Setup Topic
    ReceiveMessage(client_sock, buffer, false);
    PrintLine("The chosen topic: %s\n", buffer);
    SendAck(client_sock);

    // Setup Phrases
    SetGuessPhrase(&player, client_sock);
    SetPhrase(&player, client_sock);


    PrintPlayer(player);

    ReceiveMessage(client_sock, buffer, false);
    SetOpponentProgress(&player, *buffer, client_sock);

    // Close Connection
    close(client_sock);

    refresh();
    getch();
    endwin();
    return 0;
}

void setupSocketConnection(int *client_sock, int port, char *host){
    struct sockaddr_in server_addr;
    struct hostent *server;
    CreateSocket(client_sock);
    server = FindHost(host);
    EstablishConnection(*client_sock, server, port);

    PrintLine("Connected to Player 1.\n");
    usleep(2000);
    system("clear");
}
