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
#include "draw_box.h"
#include <string.h>
#include <ncurses.h>

void setupSocketConnection(int *server_sock, int *client_sock, int port);
char *setupTopic();

int main(int argc, char *argv[]){
    initscr();
    int rows = 125;
    int cols = 125;
    resize_term(rows, cols);
    int server_sock, client_sock, port_no, n;
   
    bool isGuessing = true;
    char buffer[MAX_STRING_SIZE];
    raw();
    keypad(stdscr, TRUE);

    // Draw a box around the terminal window
    box(stdscr, 0, 0);

    // Refresh the screen
    refresh();

    PrintFile("assets/title.txt");

    // Setup Connection
    ValidateArgs(argv[0], 2, argc);
    setupSocketConnection(&server_sock, &client_sock, atoi(argv[1]));

    struct Player player = CreatePlayer();

    // Setup Topic
    SendMessage(client_sock, setupTopic(), false);
    ReceiveAck(client_sock);

    // Setup Phrases
    SetPhrase(&player, client_sock); 
    SetGuessPhrase(&player, client_sock);

    PrintPlayer(player);

    while (player.score > 0) {
        if (isGuessing) {
            PrintLine("Your turn to guess.\n");
            char letter = InputLetter(&player);
            if (SetProgress(&player, letter, client_sock)) break;
        } else {
            PrintLine("Your opponent is guessing.\n");
            ReceiveMessage(client_sock, buffer, false);
            if (SetOpponentProgress(&player, *buffer, client_sock)) break;
        }
        isGuessing = !isGuessing;
        PrintLine("\n");
    }

    // Close Connection
    close(client_sock);
    close(server_sock);

    refresh();
    getch();
    endwin();
    
    return 0; 
}

void setupSocketConnection(int *server_sock, int *client_sock, int port){
    struct sockaddr_in server_addr, client_addr;
    CreateSocket(server_sock);
    server_addr = *CreateSocketAddress(port);
    BindSocket(server_sock, &server_addr);
    socklen_t client_size = sizeof(client_addr);
    *client_sock = HandleNewConnection(*server_sock, &client_addr, &client_size);

    napms(500);
    PrintSysMessage(2, "Creating socket...");
    PrintSysMessage(1, "Socket created and listening to port.");
    PrintSysMessage(0, "Connected to Player 2.");
    napms(500);

    PrintSysMessage(3, "Creating socket...");
    PrintSysMessage(2, "Socket created and listening to port.");
    PrintSysMessage(1, "Connected to Player 2.");
    PrintSysMessage(0, "Press any key to continue...");
    getch();
    system("clear");   
}

char *setupTopic() {
    time_t t;
    srand((unsigned) time(&t));

    char *topic = topics[rand() % MAX_TOPIC];
    PrintLine("The chosen topic: %s\n", topic); 
    return topic;
}
