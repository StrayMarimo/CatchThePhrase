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

    initscr();
    int rows = 125;
    int cols = 125;
    resize_term(rows, cols);

    int client_sock;
    char buffer[MAX_STRING_SIZE];
    bool isGuessing = false;

    raw();
    keypad(stdscr, TRUE);

    // Draw a box around the terminal window
    box(stdscr, 0, 0);

    // Refresh the screen
    refresh();

    PrintFile("assets/title.txt");

    // Setup Connection
    ValidateArgs(argv[0], 3, argc);
    setupSocketConnection(&client_sock, atoi(argv[2]), argv[1]);

    struct Player player = CreatePlayer();

    // Setup Topic
    ReceiveMessage(client_sock, buffer, false);
    // PrintLine("The chosen topic: %s\n", buffer);
    SendAck(client_sock);

    // Setup Phrases
    SetGuessPhrase(&player, client_sock);
    SetPhrase(&player, client_sock);
    PrintPlayer(player);


    while (player.score > 0) {
        if (isGuessing) {
            // PrintLine("Your turn to guess.\n");
            char letter = InputLetter(&player);
            if (SetProgress(&player, letter, client_sock)) break;
        } else {
            // PrintLine("Your opponent is guessing.\n");
            ReceiveMessage(client_sock, buffer, false);
            if (SetOpponentProgress(&player, *buffer, client_sock)) break;
        }
        isGuessing = !isGuessing;
        // PrintLine("\n");
    }

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

    napms(500);
    PrintSysMessage(2, "Creating socket...");
    PrintSysMessage(1, "Connected to Player 1.");
    PrintSysMessage(0, "Press any key to continue...");
    getch();
    system("clear");
}
