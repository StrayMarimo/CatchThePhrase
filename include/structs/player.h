#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>

struct Player {
    int score;
    char player_phrase[MAX_STRING_SIZE];
    char opponent_phrase[MAX_STRING_SIZE];
    char progress[MAX_STRING_SIZE];
    char opponent_progress[MAX_STRING_SIZE];
    char letters_pressed[26];
};

struct Player CreatePlayer();
void PrintPlayer(struct Player player);
void SetPhrase(struct Player *player, int client_sock);
void SetGuessPhrase(struct Player *player, int client_sock);
char InputLetter(struct Player *player);
bool isLetterPressed(struct Player *player, char letter);
bool SetOpponentProgress(struct Player *player, char letter, int client_sock);
bool SetProgress(struct Player *player, char letter, int client_sock);
#endif