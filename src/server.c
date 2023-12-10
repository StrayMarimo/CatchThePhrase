#include "raylib.h"
#include "common_utils.h"
#include "socket_connection.h"
#include "socket_communication.h"
#include "draw.h"
#include "int_values.h"
#include "string_values.h"
#include "player.h"
#include "topics.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

char system_message[MAX_STRING_SIZE];
char system_message2[MAX_STRING_SIZE];
char system_message3[MAX_STRING_SIZE];

void setupSocketConnection(int *server_sock, int *client_sock, int port);
char *setupTopic();
void promptPhrase(char topic[MAX_STRING_SIZE], int client_sock);

typedef enum GameScreen { TITLE, GAMEPLAY, GAMEOVER } GameScreen;

int main(int argc, char *argv[]) {
    int framesCounter = 0;
    char phraseBuffer[MAX_STRING_SIZE] = "\0";
    int server_sock, client_sock;
    int letterCount = 0;
    char topic[MAX_STRING_SIZE];
    char winner[MAX_STRING_SIZE];

    bool is_setting_topic = true;
    bool is_setting_phrase = false;
    bool is_receiving_phrase = false;
    bool is_guessing = false;
    bool is_waiting_for_guess = false;
    bool did_player1_won = false;

     // Setup Connection
    ValidateArgs(argv[0], 2, argc);
    setupSocketConnection(&server_sock, &client_sock, atoi(argv[1]));
    struct Player player = CreatePlayer();
    printf("Player created\n: %s", player.letters_pressed);

    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE_PLAYER1);
    Rectangle textBox = { 30, 440, (SCREEN_WIDTH - 60) / 3 * 2 - 20, 10  };
    bool mouseOnText = false;
    GameScreen currentScreen = TITLE;
    SetTargetFPS(60);

    // Update
    while (!WindowShouldClose()) {
        switch (currentScreen) {
            case TITLE:
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = GAMEPLAY;
                    promptPhrase(topic, client_sock);
                }
                break;
            case GAMEPLAY:
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = TITLE;
                if (is_setting_topic) ToggleFlags(&is_setting_topic, &is_setting_phrase);

                if (is_receiving_phrase) {
                    SetGuessPhrase(&player, client_sock);
                    ToggleFlags(&is_receiving_phrase, &is_guessing);
                    framesCounter = 0;
                    AddSystemMessage(GUESS_PHRASE);
                }

                if (is_waiting_for_guess) {
                    if (!SetOpponentProgress(&player, client_sock))
                        ToggleFlags(&is_waiting_for_guess, &is_guessing);
                    else currentScreen = GAMEOVER;
                }

                if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
                else mouseOnText = false;
                
                if (mouseOnText && is_setting_phrase) {
                    ProcessInputForPhrase(phraseBuffer, &letterCount, &is_setting_phrase, &is_receiving_phrase, &framesCounter, &mouseOnText, client_sock, &player, true);
                }  else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                if (mouseOnText && is_guessing) { 
                    if (ProcessInputForLetter(phraseBuffer, &letterCount, &framesCounter, &mouseOnText, &is_guessing, &is_waiting_for_guess, client_sock, &player)) {
                        ReceiveAck(client_sock);
                        did_player1_won = true;
                        currentScreen = GAMEOVER;
                    }
                } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                if (mouseOnText) framesCounter++;
                else framesCounter = 0;

                framesCounter++;
                break; 
            default:
                break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);  

        switch (currentScreen) {
            case TITLE:
                ClearBackground(BLACK);
                DrawTextCenter(PHRASE, 220, 50, WHITE);
                DrawTextCenter(TextSubtext(PRESS_ENTER, 0, framesCounter / 5), 280, 20, DARKGRAY);
                framesCounter++;
                break;
            case GAMEPLAY:
                DrawGameLayout(&player);
                DrawTopic(topic);
                DrawInputBox(phraseBuffer, textBox, mouseOnText, letterCount, framesCounter);
                break;
            case GAMEOVER:
                ClearBackground(BLACK);
                strcpy(winner, WON);
                strcat(winner, did_player1_won ? "1" : "2");
                strcat(winner, " WON!");
                DrawTextCenter(winner, 220, 20, GREEN);
                DrawTextCenter(EXITING, 250, 10, DARKGRAY);
            default:
                break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
void setupSocketConnection(int *server_sock, int *client_sock, int port){
    struct sockaddr_in server_addr, client_addr;
    CreateSocket(server_sock);
    server_addr = *CreateSocketAddress(port);
    BindSocket(server_sock, &server_addr);
    socklen_t client_size = sizeof(client_addr);
    *client_sock = HandleNewConnection(*server_sock, &client_addr, &client_size);
}

void promptPhrase(char topic[MAX_STRING_SIZE], int client_sock) {
    strcpy(system_message, DISPLAY_TOPIC);
    strcpy(topic, CapitalizePhrase(setupTopic()));
    strcat(system_message, topic);
    SendMessage(client_sock, setupTopic());
    ReceiveAck(client_sock);
    strcpy(system_message2, system_message);
    strcpy(system_message, SET_PHRASE);
}

char *setupTopic() {
    time_t t;
    srand((unsigned) time(&t));
    char *topic = topics[rand() % MAX_TOPIC];
    return topic;
}

