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
#include "string_values.h"

char system_message[MAX_STRING_SIZE];
char system_message2[MAX_STRING_SIZE];
char system_message3[MAX_STRING_SIZE];


typedef enum GameScreen { TITLE, GAMEPLAY } GameScreen;
void setupSocketConnection(int *client_sock, int port, char *host);
void getTopic(int client_sock, char *topic);

int main(int argc,  char *argv[]){
    int framesCounter = 0;
    int client_sock;
    char buffer[MAX_STRING_SIZE];
    char topic[MAX_STRING_SIZE];
    char phraseBuffer[MAX_STRING_SIZE] = "\0";
    int letterCount = 0;
    bool isGuessing = false;
    bool is_receiving_phrase = false;
    bool is_receiving_topic = true;
    bool is_setting_phrase = false;
    struct Player player = CreatePlayer();

    // Setup Connection
    ValidateArgs(argv[0], 3, argc);
    setupSocketConnection(&client_sock, atoi(argv[2]), argv[1]);
    
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE_PLAYER2);
    Rectangle textBox = { 30, 440, (SCREEN_WIDTH - 60) / 3 * 2 - 20, 10  };
    bool mouseOnText = false;

    GameScreen currentScreen = TITLE;
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        switch (currentScreen) {
            case TITLE:
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = GAMEPLAY;
                    getTopic(client_sock, topic); 
                }
                break;
            case GAMEPLAY:
                // Listen to ESC button
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = TITLE;

                if (is_receiving_topic) ToggleFlags(&is_receiving_topic, &is_receiving_phrase);

                if (is_receiving_phrase) {
                    SetGuessPhrase(&player, client_sock);
                    ToggleFlags(&is_receiving_phrase, &is_setting_phrase);
                }

                if (CheckCollisionPointRec(GetMousePosition(), textBox) && is_setting_phrase) mouseOnText = true;
                else mouseOnText = false;


                if (mouseOnText) 
                    ProcessInputForPhrase(phraseBuffer, &letterCount, &is_setting_phrase, &is_receiving_phrase, &framesCounter, &mouseOnText, client_sock, &player);
                
                else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                if (mouseOnText) framesCounter++;
                else framesCounter = 0;

                framesCounter++;
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
                DrawGameLayout(&player);
                DrawInputBox(phraseBuffer, textBox, mouseOnText, letterCount, framesCounter);
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

void getTopic(int client_sock, char *topic) {
    strcpy(system_message, DISPLAY_TOPIC);
    ReceiveMessage(client_sock, topic);
    strcpy(topic, CapitalizePhrase(topic));
    strcat(system_message, CapitalizePhrase(topic));
    strcpy(system_message2, system_message);
    strcpy(system_message, WAITING_FOR_PHRASE);
    SendAck(client_sock);
}
