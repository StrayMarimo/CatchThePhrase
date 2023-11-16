#ifndef PLAYER_H
#define PLAYER_H

struct Player {
    int score;
    char phrase[MAX_STRING_SIZE];
    char guessphrase[MAX_STRING_SIZE];
    char progress[MAX_STRING_SIZE];
};

struct Player CreatePlayer();
void SetPhrase(struct Player *player, int client_sock);
void SetGuessPhrase(struct Player *player, int client_sock);
#endif