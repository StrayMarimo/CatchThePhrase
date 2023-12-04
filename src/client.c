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
char phraseBuffer[MAX_STRING_SIZE] = "\0";
int letterCount = 0;

typedef enum GameScreen { TITLE, GAMEPLAY } GameScreen;
void setupSocketConnection(int *client_sock, int port, char *host);

int main(int argc,  char *argv[]){
    int framesCounter = 0;
    int client_sock;
    char buffer[MAX_STRING_SIZE];
    char topic[MAX_STRING_SIZE];
    bool isGuessing = false;
    struct Player player = CreatePlayer();

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

                if (strlen(topic) == 0) {
                    strcpy(system_message, "System Message> The chosen topic is: ");
                    ReceiveMessage(client_sock, topic);
                    strcat(system_message, topic);
                    strcpy(system_message2, system_message);
                    strcpy(system_message, "System Message> Waiting for opponent to set a phrase...");
                    SendAck(client_sock);

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
                DrawHealthBar(player.score, 30, SCREEN_HEIGHT / 2);
                DrawHealthBar(player.opponent_score, (SCREEN_WIDTH - 30) / 2 + 40, SCREEN_HEIGHT / 2);
                DrawTopic(topic);
                DrawLettersPressed(player.letters_pressed, 50);
                DrawLettersPressed(player.opponent_letters_pressed, 520);
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
