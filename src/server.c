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

char system_message[MAX_STRING_SIZE];
char system_message2[MAX_STRING_SIZE];
char system_message3[MAX_STRING_SIZE];

void setupSocketConnection(int *server_sock, int *client_sock, int port);
char *setupTopic();
void promptPhrase(char topic[MAX_STRING_SIZE], int client_sock);

typedef enum GameScreen { TITLE, GAMEPLAY } GameScreen;

int main(int argc, char *argv[]) {
    int framesCounter = 0;
    char phraseBuffer[MAX_STRING_SIZE] = "\0";
    int server_sock, client_sock;
    int letterCount = 0;
    char topic[MAX_STRING_SIZE];

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
                if (IsKeyPressed(KEY_ENTER)) currentScreen = GAMEPLAY;
                break;
            case GAMEPLAY:
                if (IsKeyPressed(KEY_ESCAPE)) currentScreen = TITLE;
                if (strlen(topic) == 0) promptPhrase(topic, client_sock);
                if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
                else mouseOnText = false;
                if (mouseOnText && player.player_phrase[0] == '\0') {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    GetInput(&letterCount, phraseBuffer);
                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        letterCount--;
                        if (letterCount < 0) letterCount = 0;
                        phraseBuffer[letterCount] = '\0';
                    }
                    if (IsKeyPressed(KEY_ENTER)) {
                        strcpy(player.player_phrase, phraseBuffer);
                        char new_message[MAX_STRING_SIZE] = DISPLAY_PHRASE;
                        strcat(new_message, player.player_phrase);
                        AddSystemMessage(new_message);
                        ClearInputBox(phraseBuffer);
                        SetPhrase(&player, client_sock);
                        framesCounter = 0;
                    }
                }
                else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
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
                DrawTextCenter(PHRASE, 220, 20, MAROON);
                DrawTextCenter(TextSubtext(PRESS_ENTER, 0, framesCounter / 5) ,250, 10, DARKGRAY);
                framesCounter++;
                break;
            case GAMEPLAY:
                DrawGameLayout();
                DrawTopic(topic);
                DrawHealthBar(player.score, 30, SCREEN_HEIGHT / 2);
                DrawHealthBar(player.opponent_score, (SCREEN_WIDTH - 30) / 2 + 40, SCREEN_HEIGHT / 2);
                DrawText(system_message3, 30, 380, 10, BLACK);
                DrawText(system_message2, 30, 400, 10, BLACK);
                DrawText(system_message, 30, 420, 10, MAROON);
                DrawLettersPressed(player.letters_pressed, 50);
                DrawLettersPressed(player.opponent_letters_pressed, 520);
                DrawText(player.opponent_progress, (SCREEN_WIDTH - 30) / 2 + 50, 50, 20, DARKGRAY);
                DrawInputBox(phraseBuffer, textBox, mouseOnText, letterCount, framesCounter);
                break;
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
    strcpy(topic, setupTopic());
    strcat(system_message, topic);
    SendMessage(client_sock, setupTopic());
    ReceiveAck(client_sock);
    strcpy(system_message2, system_message);
    strcpy(system_message, SET_PHRASE);
}

// #include <stdlib.h>
// #include <stdio.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <time.h>
// #include "int_values.h"
// #include "socket_connection.h"
// #include "socket_communication.h"
// #include "common_utils.h"
// #include "topics.h"
// #include "player.h"
// #include "draw_box.h"
// #include <string.h>
// #include <ncurses.h>

// void setupSocketConnection(int *server_sock, int *client_sock, int port);
// char *setupTopic();

// int main(int argc, char *argv[]){
//     initscr();
//     int rows = 125;
//     int cols = 125;
//     resize_term(rows, cols);
//     int server_sock, client_sock, port_no, n;

//     bool isGuessing = true;
//     char buffer[MAX_STRING_SIZE];
//     raw();
//     keypad(stdscr, TRUE);

//     // Draw a box around the terminal window
//     box(stdscr, 0, 0);

//     // Refresh the screen
//     refresh();

//     PrintFile("assets/title.txt");

//     // Setup Connection
//     ValidateArgs(argv[0], 2, argc);
//     setupSocketConnection(&server_sock, &client_sock, atoi(argv[1]));

//     struct Player player = CreatePlayer();

//     // Setup Topic
//     SendMessage(client_sock, setupTopic(), false);
//     ReceiveAck(client_sock);

//     // Setup Phrases
//     SetPhrase(&player, client_sock); 
//     SetGuessPhrase(&player, client_sock);

//     PrintPlayer(player);

//     while (player.score > 0) {
//         if (isGuessing) {
//             PrintLine("Your turn to guess.\n");
//             char letter = InputLetter(&player);
//             if (SetProgress(&player, letter, client_sock)) break;
//         } else {
//             PrintLine("Your opponent is guessing.\n");
//             ReceiveMessage(client_sock, buffer, false);
//             if (SetOpponentProgress(&player, *buffer, client_sock)) break;
//         }
//         isGuessing = !isGuessing;
//         PrintLine("\n");
//     }

//     // Close Connection
//     close(client_sock);
//     close(server_sock);

//     refresh();
//     getch();
//     endwin();
    
//     return 0; 
// }

// void setupSocketConnection(int *server_sock, int *client_sock, int port){
//     struct sockaddr_in server_addr, client_addr;
//     CreateSocket(server_sock);
//     while (true) {
//         server_addr = *CreateSocketAddress(port); 
//         if (server_addr.sin_port > 0) {
//             printf("%d,%d", *server_sock, server_addr.sin_port);
//             break;
//         } else if (framesCounter > 10) {
//             DrawTextCenter(TextSubtext(BIND_ERROR, 0, framesCounter / 10 ), 290, 10, RED);
//             DieWithError();
//         }
//         framesCounter++;
//     }
    
  

//     BindSocket(server_sock, &server_addr);
//     // while (true){
//     //     // Create Socket
//     //     bool is_socket_created = CreateSocket(server_sock);
//     //     if (!is_socket_created && framesCounter > 10) {
//     //         DrawTextCenter(TextSubtext(SOCKET_ERROR, 0, framesCounter / 10 ), 270, 10, RED);
//     //         DieWithError();
//     //     } else if (is_socket_created) {
//     //         DrawTextCenter(TextSubtext(CREATING_SOCKET, 0, framesCounter / 5 ), 270, 10, DARKGRAY);
//     //         while (true) {
//     //             // Create Socket Address
//     //             server_addr = *CreateSocketAddress(port);
//     //             if (server_addr.sin_port <= 0 && framesCounter > 10) {
//     //                 DrawTextCenter(TextSubtext(BIND_ERROR, 0, framesCounter / 10 ), 290, 10, RED);
//     //                 DieWithError();
//     //             } else if (server_addr.sin_port >= 0) {
//     //                 // Bind Socket
//     //                 while (true){
//     //                     bool is_socket_bound = BindSocket(server_sock, &server_addr);
//     //                     if (!is_socket_bound && framesCounter > 10) {  
//     //                         DrawTextCenter(TextSubtext(BIND_ERROR, 0, framesCounter / 10 ), 290, 10, RED);
//     //                         DieWithError();
//     //                     } else if (is_socket_bound) {
//     //                         DrawTextCenter(TextSubtext(LISTENING_SOCKET, 0, framesCounter / 10 ), 290, 10, DARKGRAY);
//     //                         break;
//     //                     }
//     //                     framesCounter++;
//     //                 }
//     //                 break;
//     //             }
//     //             printf("%d,%d", *server_sock, server_addr.sin_port);

//     //             framesCounter++;
//     //         }
//     //         break;
//     //     }

//     //     framesCounter++;
//     }

    // while (server_addr.sin_port <= 0 && *server_sock <= 0) {
    //     server_addr = *CreateSocketAddress(port);
    //     if (server_addr.sin_port <= 0 && *server_sock <= 0 && framesCounter > 20) {
    //         DrawTextCenter(TextSubtext(BIND_ERROR, 0, framesCounter / 10 ), 290, 10, RED);
    //         DieWithError();
    //     }
    //     if (server_addr.sin_port >= 0 && *server_sock >= 0) {
    //         printf("%d,%d", *server_sock, server_addr.sin_port);
    //         // while (true){
    //         //     if (BindSocket(server_sock, &server_addr)) {
    //         //         DrawTextCenter(TextSubtext(LISTENING_SOCKET, 0, framesCounter / 10 ), 290, 10, DARKGRAY);
    //         //         break;
    //         //     } else if(framesCounter > 20) {
    //         //         DrawTextCenter(TextSubtext(BIND_ERROR, 0, framesCounter / 10 ), 290, 10, RED);
    //         //         DieWithError();
    //         //     }
    //         // }
    //         break;
    //     }

    // }
    
    // socklen_t client_size = sizeof(client_addr);
    // *client_sock = HandleNewConnection(*server_sock, &client_addr, &client_size);

    // //napmss(500);
    // PrintSysMessage(2, "Creating socket...");
    // PrintSysMessage(1, "Socket created and listening to port.");
    // PrintSysMessage(0, "Connected to Player 2.");
    // //napmss(500);

    // PrintSysMessage(3, "Creating socket...");
    // PrintSysMessage(2, "Socket created and listening to port.");
    // PrintSysMessage(1, "Connected to Player 2.");
    // PrintSysMessage(0, "Press any key to continue...");
    // getch();
    // system("clear");   


char *setupTopic() {
    time_t t;
    srand((unsigned) time(&t));
    char *topic = topics[rand() % MAX_TOPIC];
    return topic;
}
