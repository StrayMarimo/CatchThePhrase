#include "int_values.h"
#include <stdio.h>
#include "socket_communication.h"
#include <string.h>
#include "power_up.h"
#include "sound_manager.h"
#include <stdbool.h>

#ifndef PLAYER_H
#define PLAYER_H


// extern char system_message[MAX_STRING_SIZE];
struct Player {
    int score;
    int opponent_score;
    char player_phrase[MAX_STRING_SIZE];
    char opponent_phrase[MAX_STRING_SIZE];
    char progress[MAX_STRING_SIZE];
    char opponent_progress[MAX_STRING_SIZE];
    char letters_pressed[26];
    char opponent_letters_pressed[26];
    int consecutiveCorrectGuesses;
    int consecutiveCorrectGuessesOpponent;
    char turn[MAX_STRING_SIZE];
};

struct Player CreatePlayer();
void PrintPlayer(struct Player player);
void SetPhrase(struct Player *player, int client_sock);
void SetGuessPhrase(struct Player *player, int client_sock);
bool IsPhraseGuessed(char *phrase, char *progress);
bool UpdateProgress(struct Player *player, char letter, bool isPlayer);
bool isLetterPressed(struct Player *player, char letter);
void UpdateLetterPressed(struct Player *player, char letter, bool isPlayer);
bool SetOpponentProgress(struct Player *player, int client_sock, struct Audios *audio);
bool SetProgress(struct Player *player, char letter, int client_sock, bool *isGuessing, bool *isWaitingForGuess, struct Audios *audio);
#endif