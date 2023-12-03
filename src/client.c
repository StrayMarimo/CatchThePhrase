#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket_connection.h"
#include "socket_communication.h"
#include "int_values.h"
#include "common_utils.h"
#include "draw.h"
#include "player.h"
#include <string.h>
#include "raylib.h"

char system_message[MAX_STRING_SIZE];
char system_message2[MAX_STRING_SIZE];
char system_message3[MAX_STRING_SIZE];
typedef enum GameScreen { TITLE, GAMEPLAY } GameScreen;
void setupSocketConnection(int *client_sock, int port, char *host);

int main(int argc,  char *argv[]){
    int framesCounter = 0;
    int client_sock;
    bool is_topic_set = false;
    char buffer[MAX_STRING_SIZE];
    char topic[MAX_STRING_SIZE];
    bool isGuessing = false;
    // struct Player player = CreatePlayer();

    // Setup Connection
    ValidateArgs(argv[0], 3, argc);
    setupSocketConnection(&client_sock, atoi(argv[2]), argv[1]);
    
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE_PLAYER2);

    GameScreen currentScreen = TITLE;
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        switch (currentScreen) {
            case TITLE:
                if (IsKeyPressed(KEY_ENTER)) currentScreen = GAMEPLAY;
                break;
            case GAMEPLAY:
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = TITLE;

                if (is_topic_set == false) {
                    ReceiveMessage(client_sock, topic);
                    // strcpy(system_message, buffer);
                    SendAck(client_sock);
                    is_topic_set = true;
                }
                break;
            default:
                break;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);  

        switch (currentScreen) {
            case TITLE:
                DrawTextCenter(PHRASE, 220, 20, MAROON);
                DrawTextCenter(TextSubtext(PRESS_ENTER, 0, framesCounter / 5) ,250, 10, DARKGRAY);
                framesCounter++;
                break;
            case GAMEPLAY:
                DrawGameLayout();
                DrawText(system_message3, 30, 380, 10, BLACK);
                DrawText(system_message2, 30, 400, 10, BLACK);
                DrawText(system_message, 30, 420, 10, MAROON);
                // DrawHealthBar(player.score);
                DrawTopic(topic);
                break;
            default:
                break;
        }

        EndDrawing();
    }
    CloseWindow();

    // struct Player player = CreatePlayer();

    // // Setup Topic
    // ReceiveMessage(client_sock, buffer, false);
    // // PrintLine("The chosen topic: %s\n", buffer);
    // SendAck(client_sock);

    // // Setup Phrases
    // SetGuessPhrase(&player, client_sock);
    // SetPhrase(&player, client_sock);
    // PrintPlayer(player);


    // while (player.score > 0) {
    //     if (isGuessing) {
    //         // PrintLine("Your turn to guess.\n");
    //         char letter = InputLetter(&player);
    //         if (SetProgress(&player, letter, client_sock)) break;
    //     } else {
    //         // PrintLine("Your opponent is guessing.\n");
    //         ReceiveMessage(client_sock, buffer, false);
    //         if (SetOpponentProgress(&player, *buffer, client_sock)) break;
    //     }
    //     isGuessing = !isGuessing;
    //     // PrintLine("\n");
    // }

    // Close Connection
    // close(client_sock);

    // refresh();
    // getch();
    // endwin();
    return 0;
}

void setupSocketConnection(int *client_sock, int port, char *host){
    struct sockaddr_in server_addr;
    struct hostent *server;
    CreateSocket(client_sock);
    server = FindHost(host);
    EstablishConnection(*client_sock, server, port);
}
